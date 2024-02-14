#include "schgrid.h"
#include "event.h"
#include <SDL_log.h>
#include <SDL_scancode.h>
#include <SDL_video.h>
GridConf gridconf;

void Grid_init(int windowWidth, int WindowHeight)
{
//    SDL_Color color = {255, 0, 0, 80};
   // SDL_Color color = {202, 209, 207, 0.62*255};
    SDL_Color color = {170, 203, 205, 0.82*255};
    gridconf.color = color;
    gridconf.scale = 0.25;
    gridconf.position.x = 260;
    gridconf.position.y = 55;

    gridconf.width = 1;

    //网格显示区域
    SDL_Rect rect = {0,0,windowWidth,WindowHeight};
    gridconf.drawArea = rect;

    gridconf.cursor.x = 20;
    gridconf.cursor.y = 20;

}


void GridScale(float deltaScale, SDL_Window* w)
{
    float distance = GRID_INITIAL_DIS * gridconf.scale;
    float cursor_position_X = gridconf.cursor.x*distance + gridconf.position.x;
    float cursor_position_Y = gridconf.cursor.y*distance + gridconf.position.y;
    SDL_WarpMouseInWindow(w, cursor_position_X, cursor_position_Y);

    float scale_pre = gridconf.scale + ((deltaScale>0)?1:-1)*gridconf.scale*0.1;

    if(scale_pre > 0.01 && scale_pre < 10)
        gridconf.scale = scale_pre;

    distance = GRID_INITIAL_DIS * gridconf.scale;
    float new_position_X = -gridconf.cursor.x*distance + cursor_position_X;
    float new_position_Y = -gridconf.cursor.y*distance + cursor_position_Y;

    Grid_set_position(new_position_X, new_position_Y);

}

void Grid_draw(SDL_Renderer* renderer)
{
    float distance = GRID_INITIAL_DIS * gridconf.scale;
    
    //网格距离到达10时，隐藏一部分
    int HIDE_Factor= 1;
    if (distance<10) 
        HIDE_Factor = 10;
    
    if (distance<1)
        HIDE_Factor = 100;


    SDL_SetRenderDrawColor(renderer, gridconf.color.r, gridconf.color.g, gridconf.color.b, gridconf.color.a);
    //纵线
    //起点网格坐标为：x = (0 - position.x)/distance
    //终点:           x = (W - position.x)/distance
    int i_initial = (0 - gridconf.position.x)/distance;
    int i_final = (gridconf.drawArea.w - gridconf.position.x)/distance;
    SDL_FPoint verical_line_points[SDL_abs(i_final-i_initial)*2+2];

    int help[4] = {-1, gridconf.drawArea.h+50,gridconf.drawArea.h+50,-1};
    int k = -1;
    for (int i = i_initial/HIDE_Factor*HIDE_Factor; i<=i_final; i=i+HIDE_Factor)
    {
            float x = i*distance+gridconf.position.x;
            k++;
            verical_line_points[k].x = x; 
            verical_line_points[k].y = help[k%4];
            k++;
            verical_line_points[k].x = x; 
            verical_line_points[k].y = help[k%4];
    }
    int verical_line_numbers = k+1;

    //水平线
    //起点网格坐标 ： y =  (0 - position.y)/distance
    //终点:           y = (H - position.y)/distance
    i_initial = (0 - gridconf.position.y)/distance;
    i_final = (gridconf.drawArea.h - gridconf.position.y)/distance;
    SDL_FPoint horizontal_line_points[SDL_abs(i_final-i_initial)*2+2];

    int help__[4] = {-1, gridconf.drawArea.w+50,gridconf.drawArea.w+50,-1};
    k = -1;
    for (int i = i_initial/HIDE_Factor*HIDE_Factor; i<=i_final; i=i+HIDE_Factor) 
    {
            float y = i*distance+gridconf.position.y;
            k++;
            horizontal_line_points[k].x = help__[k%4]; 
            horizontal_line_points[k].y = y;
            k++;
            horizontal_line_points[k].x = help__[k%4]; 
            horizontal_line_points[k].y = y;
    }
    int horizontal_line_numbers = k+1;

    SDL_RenderDrawLinesF(renderer, verical_line_points, verical_line_numbers); 
    SDL_RenderDrawLinesF(renderer, horizontal_line_points, horizontal_line_numbers); 

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, gridconf.cursor.x * distance + gridconf.position.x, 0, gridconf.cursor.x * distance+gridconf.position.x, gridconf.drawArea.h);
    SDL_RenderDrawLine(renderer, 0, gridconf.cursor.y*distance + gridconf.position.y, gridconf.drawArea.w, gridconf.cursor.y * distance+gridconf.position.y);
}

void Grid_set_cursor(int mouseX, int mouseY)
{
    float grid_x_f = (mouseX - gridconf.position.x) / (GRID_INITIAL_DIS*gridconf.scale);
    int grid_x_f_int = grid_x_f;
    float grid_y_f = (mouseY - gridconf.position.y) / (GRID_INITIAL_DIS*gridconf.scale);
    int grid_y_f_int = grid_y_f;

    float delta_x_f = grid_x_f - grid_x_f_int;
    float delta_y_f = grid_y_f - grid_y_f_int;

    int grid_x,grid_y;
    if ( ((delta_x_f < 0.5)&&(delta_x_f >= 0))||( (delta_x_f < 0)&&(delta_x_f >= -0.5) ) ) 
    {
        grid_x = grid_x_f_int;
    }
    else {
        int plus = (delta_x_f>0)?1:-1;
        grid_x = grid_x_f_int+plus;
    }

    if ( ((delta_y_f < 0.5)&&(delta_y_f >= 0))||( (delta_y_f < 0)&&(delta_y_f >= -0.5) ) ) 
    {
        grid_y = grid_y_f_int;
    }
    else {
        int plus = (delta_y_f>0)?1:-1;
        grid_y = grid_y_f_int+plus;
    }

    gridconf.cursor.x = grid_x;
    gridconf.cursor.y = grid_y;
}

void Grid_set_position(float x, float y)
{
    gridconf.position.x = x;
    gridconf.position.y = y;
}
void Grid_move_position(float x, float y)
{
    gridconf.position.x += x;
    gridconf.position.y += y;
}
float Grid_get_distance()
{
    return GRID_INITIAL_DIS*gridconf.scale;
}
SDL_FPoint Grid_get_position()
{
    return gridconf.position;
}
struct cursor Grid_get_cursor()
{
    return gridconf.cursor;
}


void GridEventProcess(SDL_Window *w)
{
    int motion_x, motion_y, position_x, position_y;
    Mouse_get_motion(&motion_x, &motion_y);
    Mouse_get_position(&position_x, &position_y);
    if (Mouse_is_wheeled()) 
    {
        GridScale(Mouse_get_wheel(), w);
    }
    else if (Mouse_is_moved()) 
    {
        if (Mouse_is_pressed(3)) 
            Grid_move_position(motion_x, motion_y);
        Grid_set_cursor(position_x, position_y);
    }
}

