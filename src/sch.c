#include "sch.h"
#include "event.h"
#include "schgrid.h"
#include <SDL_log.h>
#include <SDL_mouse.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <stdlib.h>
#include <string.h>

SchElements schElements;

Wire defaultWire = 
{
    .color = {0,150, 0, 255},
    .width = 3,
    .netID = 0,
    .divide_points = NULL,
    .divide_counts = 0,
};

static int dot_in_segment_not_start_and_end(int point[2], int startpoint[2], int endpoint[2]);
static int dot_in_segment_start_and_end(int point[2], int startpoint[2], int endpoint[2]);
static void parse_divide_wires();

void Sch_init()
{
    schElements.wires = (Wire*)calloc(sizeof(Wire), 1);
    schElements.wires[0] = defaultWire;
    schElements.wire_counts = 0;
}

void Sch_createWire(int x, int y)
{
    schElements.wires = (Wire*)realloc(schElements.wires, sizeof(Wire)*(schElements.wire_counts+1));
    schElements.wire_counts +=1;
    schElements.wires[schElements.wire_counts-1]= defaultWire;
    schElements.wires[schElements.wire_counts-1].x0 = x;
    schElements.wires[schElements.wire_counts-1].y0 = y;
    schElements.wires[schElements.wire_counts-1].x1 = x;
    schElements.wires[schElements.wire_counts-1].y1 = y;
}



//计算线的起始坐标，并设置
void Sch_draw_wire()
{
    int x = Grid_get_cursor().x;
    int y = Grid_get_cursor().y;
    Wire *w1 = &schElements.wires[schElements.wire_counts - 2];
    Wire *w2 = &schElements.wires[schElements.wire_counts - 1];

    static int draw_wire_direction = 1;

    if ((x == w1->x0)&&(y != w1->y0)) 
    {
        draw_wire_direction = 2;
    }
    if ((x != w1->x0)&&(y == w1->y0)) 
    {
        draw_wire_direction = 1;
    }
    //SDL_Log("Draw direction = %d", draw_wire_direction);
    if (draw_wire_direction == 1) {
        w1->x1 = x;
        w1->y1 = w1->y0;

        w2->x0 = x;
        w2->y0 = w1->y0;
        
    }
    if (draw_wire_direction == 2) {
        w1->y1 = y;
        w1->x1 = w1->x0;

        w2->y0 = y;
        w2->x0 = w1->x0;
    }
    w2->x1 = x;w2->y1 = y;
}

void Sch_draw(SDL_Renderer* renderer)
{
    /*    if (schElements.wires!=NULL) 
          {
          free(schElements.wires);
          schElements.wires = NULL;
          schElements.wire_counts = 0;
          }
          */
    Wire w;
    for (int i = 0; i < schElements.wire_counts; i++) 
    {
        w = schElements.wires[i];
        
        SDL_SetRenderDrawColor(renderer, w.color.r, w.color.g, w.color.b, w.color.a);

        float x1 = w.x0 * Grid_get_distance() + Grid_get_position().x; 
        float y1 = w.y0 * Grid_get_distance() + Grid_get_position().y; 
        float x2 = w.x1 * Grid_get_distance() + Grid_get_position().x; 
        float y2 = w.y1 * Grid_get_distance() + Grid_get_position().y; 

        SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
        SDL_RenderDrawLineF(renderer, x1-0.5, y1-0.5, x2-0.5, y2-0.5);
        SDL_RenderDrawLineF(renderer, x1+0.5, y1+0.5, x2+0.5, y2+0.5);
        SDL_RenderDrawLineF(renderer, x1-0.9, y1-0.9, x2-0.9, y2-0.9);
        SDL_RenderDrawLineF(renderer, x1+0.9, y1+0.9, x2+0.9, y2+0.9);

    }

}

enum DrawWireState
{
    DRAW_WIRE_START = 0,
    DRAW_WIRE_DRAWING,
};

static void Sch_draw_wire_work()
{
    static enum DrawWireState state = DRAW_WIRE_START;

    switch_case_start(state, DRAW_WIRE_START)
            if (Key_is_clicked(SDL_SCANCODE_W)) {
                Sch_createWire(Grid_get_cursor().x, Grid_get_cursor().y);
                Sch_createWire(Grid_get_cursor().x, Grid_get_cursor().y);
                state = DRAW_WIRE_DRAWING;
            }
    case_add(DRAW_WIRE_DRAWING)
            Sch_draw_wire();
            if (Key_is_clicked(SDL_SCANCODE_ESCAPE)) { //取消画线
                schElements.wire_counts-=2;
                state = DRAW_WIRE_START;
            }
            else if (Key_is_clicked(SDL_SCANCODE_W)) { //结束画线
                parse_divide_wires();
                state = DRAW_WIRE_START;
            }
            else if (Mouse_is_clicked(1)) { //以上一条线的终点为起点创建线条
                Sch_createWire(schElements.wires[schElements.wire_counts-1].x1, 
                        schElements.wires[schElements.wire_counts-1].y1);
            }
    switch_case_end
}

void Sch_events_process()
{
    Sch_draw_wire_work();
}

static int Compare_small_big(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

static void parse_divide_wires()
{
    Wire *wires_divided = NULL;    
    int max_count = 2000;
    wires_divided = (Wire*)calloc(sizeof(Wire), max_count);

    int *ignored = (int*)calloc(sizeof(int), schElements.wire_counts);
    memset(ignored, 0, sizeof(int)*schElements.wire_counts);
    for (int i = 0; i < schElements.wire_counts; i++) 
    {
        if (ignored[i]) continue;

        int x1 = schElements.wires[i].x0, y1 = schElements.wires[i].y0, x2 = schElements.wires[i].x1, y2 = schElements.wires[i].y1;
        int start[2] = {x1, y1};
        int end[2] = {x2, y2};
        //线段是一个点
        if (x1==x2&&y1==y2) { ignored[i] = 1; continue; }

        for (int j = 0; j<schElements.wire_counts; j++) 
        {
            if (ignored[j]) continue;
            if (i==j)
                continue;
            int x1_ = schElements.wires[j].x0, y1_ = schElements.wires[j].y0; 
            int x2_ = schElements.wires[j].x1, y2_ = schElements.wires[j].y1;
            int point1[2] = {x1_, y1_}, point2[2] = {x2_, y2_};
            //j 上的点被包含
            int bei_baohan1 = dot_in_segment_start_and_end(point1, start, end); 
            int bei_baohan2 = dot_in_segment_start_and_end(point2, start, end); 

            //两条线段毫无瓜葛
            if (!bei_baohan1&&!bei_baohan2) {  continue; }
            //线j包含在线i中
            if (bei_baohan1&&bei_baohan2) { ignored[j] = 1; continue; }

            //被包含，但端点不重合
            int bei_bh_ndd1 = dot_in_segment_not_start_and_end(point1, start, end); 
            int bei_bh_ndd2 = dot_in_segment_not_start_and_end(point2, start, end); 

            //包含i的一个端点，但端点不重合
            int bh_ndd1 = dot_in_segment_not_start_and_end(start, point1, point2); 
            int bh_ndd2 = dot_in_segment_not_start_and_end(end, point1, point2); 

            //插入分割点的情况
            if (bei_bh_ndd1 && !bh_ndd1 && !bh_ndd2) {
                schElements.wires[i].divide_points = (int*)realloc(schElements.wires[i].divide_points,
                                                        (schElements.wires[i].divide_counts+1)*2*sizeof(int));
                schElements.wires[i].divide_counts++;
                schElements.wires[i].divide_points[(schElements.wires[i].divide_counts-1)*2] = x1_ - x1;
                schElements.wires[i].divide_points[(schElements.wires[i].divide_counts-1)*2+1] = y1_ - y1;
                continue;
            }
            if (bei_bh_ndd2 && !bh_ndd1 && !bh_ndd2) {
                schElements.wires[i].divide_points = (int*)realloc(schElements.wires[i].divide_points,
                                                        (schElements.wires[i].divide_counts+1)*2*sizeof(int));
                schElements.wires[i].divide_counts++;
                schElements.wires[i].divide_points[(schElements.wires[i].divide_counts-1)*2] = x2_ - x1;
                schElements.wires[i].divide_points[(schElements.wires[i].divide_counts-1)*2+1] = y2_ - y1;
                continue;
            }

            //两条线段合并的情况
            if (bei_bh_ndd1 && bh_ndd1) {
//          i:             0--------------1
//          j:  1---------------0               
                ignored[i] = 1;
                schElements.wires[j].x0 = schElements.wires[i].x1;
                schElements.wires[j].y0 = schElements.wires[i].y1;
                continue;
            }
            if (bei_bh_ndd1 && bh_ndd2) {
 //          i:             1--------------0                
 //          j:  1---------------0               
                ignored[i] = 1;
                schElements.wires[j].x0 = schElements.wires[i].x0;
                schElements.wires[j].y0 = schElements.wires[i].y0;
                continue;
            }
            if (bei_bh_ndd2 && bh_ndd1) {
 //          i:             0--------------1                
 //          j:  0---------------1               
                ignored[i] = 1;
                schElements.wires[j].x1 = schElements.wires[i].x1;
                schElements.wires[j].y1 = schElements.wires[i].y1;
 //               SDL_Log("tongxiang bao han le yi tiao line!");
                continue;
            }
            if (bei_bh_ndd2 && bh_ndd2) {
 //          i:             1--------------0                
 //          j:  0---------------1               
                ignored[i] = 1;
                schElements.wires[j].x1 = schElements.wires[i].x0;
                schElements.wires[j].y1 = schElements.wires[i].y0;
                continue;
            }

        }
    }

    for (int i = 0; i < schElements.wire_counts; i++) 
    {
        int x1 = schElements.wires[i].x0;
        int y1 = schElements.wires[i].y0;
        int x2 = schElements.wires[i].x1;
        int y2 = schElements.wires[i].y1;
        SDL_Log("                                                                     ");
        SDL_Log("                                                                     ");
        SDL_Log("_____________________________________________________________________");
        SDL_Log("线段%d: (%d, %d)->(%d, %d) , 有%d个分割点", i, x1,y1,x2,y2,schElements.wires[i].divide_counts);

        for (int j = 0; j<schElements.wires[i].divide_counts; j++) 
        {
            SDL_Log("(%d,%d)",schElements.wires[i].divide_points[2*j],schElements.wires[i].divide_points[2*j+1]);
        
        }
        SDL_Log("                                                                     ");
        SDL_Log("                                                                     ");

    }


    int wire_index = 0;
    for (int i = 0; i < schElements.wire_counts; i++) 
    {
        if (ignored[i]) continue;
        if (schElements.wires[i].divide_counts==0) 
        {
            wires_divided[wire_index] = schElements.wires[i];
//            if (schElements.wires[i].divide_points !=NULL) {
//              SDL_LogError(SDL_LOG_CATEGORY_TEST, "没有分割点，但指针不为NULL");           
//          }
            wire_index++;
        }
        else
        {
            int x0 = schElements.wires[i].x0, y0 = schElements.wires[i].y0;
            int x1 = schElements.wires[i].x1, y1 = schElements.wires[i].y1;
            int* divideP = schElements.wires[i].divide_points;
            int divide_counts = schElements.wires[i].divide_counts;
            int dx[divide_counts];
            int dy[divide_counts];
            for (int h = 0; h < divide_counts; h++) 
            {
                dx[h] = divideP[h*2];
                dy[h] = divideP[h*2+1];
            }
            SDL_qsort(dx, divide_counts, sizeof(int), Compare_small_big); //sort
            SDL_qsort(dy, divide_counts, sizeof(int), Compare_small_big); //sort

            //first divided line segment
            wires_divided[wire_index] = schElements.wires[i];
            wires_divided[wire_index].x0 = x0; 
            wires_divided[wire_index].y0 = y0; 
            wires_divided[wire_index].x1 = x0+dx[0]; 
            wires_divided[wire_index].y1 = y0+dy[0]; 
            wires_divided[wire_index].divide_counts = 0;
            wires_divided[wire_index].divide_points = NULL;
            wire_index++;
            for (int j = 1; j < divide_counts; j++) 
            {
                wires_divided[wire_index] = schElements.wires[i];
                wires_divided[wire_index].x0 = x0+dx[j-1]; 
                wires_divided[wire_index].y0 = y0+dy[j-1]; 
                wires_divided[wire_index].x1 = x0+dx[j]; 
                wires_divided[wire_index].y1 = y0+dy[j]; 
                wires_divided[wire_index].divide_counts = 0;
                wires_divided[wire_index].divide_points = NULL;
                wire_index++;
            }
            //last divided line segment
            wires_divided[wire_index] = schElements.wires[i];
            wires_divided[wire_index].x0 = x0+dx[divide_counts-1]; 
            wires_divided[wire_index].y0 = y0+dy[divide_counts-1]; 
            wires_divided[wire_index].x1 = x1; 
            wires_divided[wire_index].y1 = y1; 
            wires_divided[wire_index].divide_counts = 0;
            wires_divided[wire_index].divide_points = NULL;
            wire_index++;
            free(divideP);       
        }
    }
    free(ignored);
    free(schElements.wires);
    schElements.wires = wires_divided;
    schElements.wire_counts = wire_index;
    wire_index = 0;
}
//整理wires, junction分割wire等等
static void Sch_combine_wires()
{
    Wire *wires_combined = NULL;    
    int max_count = 1000;
    wires_combined = (Wire*)calloc(sizeof(Wire), max_count);

    for (int i = 0; i < schElements.wire_counts; i++) 
    {


    }


}


static int dot_in_segment_not_start_and_end(int point[2], int startpoint[2], int endpoint[2])
{
    int x = point[0], y = point[1];
    int x1 = startpoint[0], y1 = startpoint[1];
    int x2 = endpoint[0], y2 = endpoint[1];

    //纵线
    if (x2 == x1) 
    {
        if (x == x1) 
        {
            if ((y>y1&&y<y2) || (y>y2&&y<y1)) 
                return 1;
            return 0;
        }
        return 0;
    }
    //横线
    if (y2 == y1) 
    {
        if (y==y1) 
        {
            if ((x>x1 && x<x2) ||(x>x2 && x<x1))
                return 1;
            return 0;
        }
        return 0;
    }
    return 0;
}

static int dot_in_segment_start_and_end(int point[2], int startpoint[2], int endpoint[2])
{
    int x = point[0], y = point[1];
    int x1 = startpoint[0], y1 = startpoint[1];
    int x2 = endpoint[0], y2 = endpoint[1];

    //纵线
    if (x2 == x1) 
    {
        if (x == x1) 
        {
            if ((y>=y1&&y<=y2) || (y>=y2&&y<=y1)) 
                return 1;
            return 0;
        }
        return 0;
    }
    //横线
    if (y2 == y1) 
    {
        if (y==y1) 
        {
            if ((x>=x1 && x<=x2) ||(x>=x2 && x<=x1))
                return 1;
            return 0;
        }
        return 0;
    }
    return 0;
}

void Sch_dispose()
{
    for (int i = 0; i < schElements.wire_counts; i++) 
    {
        if (schElements.wires[i].divide_points!=NULL) 
            free(schElements.wires[i].divide_points);
    }
    free(schElements.wires);
}
