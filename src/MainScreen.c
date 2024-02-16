#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include "game.h"
#include "Widget.h"
#include "mapfont.h"
#include "sch.h"
#include "schgrid.h"

int GeometryDrawLine(int x1, int y1, int x2, int y2, SDL_Color color);

SDL_FPoint *pointsOnArc(int Ox, int Oy, int x, int y, float angle, int* pointsNumber)
{
    float minA = 42;
    int numbers = SDL_fabsf(angle)/minA + 1;
    *pointsNumber = numbers;
    SDL_FPoint *points = (SDL_FPoint*)calloc(sizeof(SDL_Point), numbers);
    SDL_FPoint *points_1 = points;
    points->x = x;
    points->y = y;
    for (int i = 1; i < numbers; i++) 
    {
        points++;
        int fuhao = (angle>0)?1:-1;
        float deltaA = fuhao * i * minA * 3.14159/180;
        float cosA = SDL_cosf(deltaA);
        float sinA = SDL_sinf(deltaA);
        points->x = x*cosA - y*sinA - Ox*cosA + Oy*sinA + Ox;
        points->y = x*sinA + y*cosA - Ox*sinA - Oy*cosA + Oy;
    }

    return points_1;
}
    int countOfPoints;
    SDL_FPoint* points;
    int countOfPoints_;
    SDL_FPoint* points_;
extern char *appBasePath;
static Game *app;
static SDL_Texture *texture;

SDL_Color bgColor;

MapFont *cnfonts;
static void create()
{
    int numm = SDL_GetNumRenderDrivers();
    SDL_Log("MainScreen: create(), render driver num : %d",numm);
    for (int i = 0; i < numm; i++)
    {
        SDL_RendererInfo ri;
        SDL_GetRenderDriverInfo(i, &ri);
        SDL_Log("driver name: %s", ri.name);
        
    }
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 1200, 800);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest);
    bgColor.r = 245;
    bgColor.g = 244;
    bgColor.b = 239;
    bgColor.a = 255;
    
    // MapFontGenerator("generate/aa/myfont.png", "generate/aa/myfont.json", "font/Deng.ttf",fontSize);
    cnfonts = newMapFont(app->renderer, "generate/aa/myfont.png");


//   points = pointsOnArc(500, 350, 500, 698, -361, &countOfPoints);
//   points_ = pointsOnArc(500, 350, 500, 651, -361, &countOfPoints_);
 points = pointsOnArc(-500, -350, -500, -680, -360, &countOfPoints);
 points_ = pointsOnArc(-500, -350, -500, -670, -360, &countOfPoints_);

    
    
//points = pointsOnArc(500, 350, 500, 680, -360, &countOfPoints);
//points_ = pointsOnArc(500, 350, 500, 670, -360, &countOfPoints_);//   /*Button Background load*/
//   SDL_Surface *button_bk_s = SDL_LoadBMP("D:/Projects/C/MinGW_SDL2/resources/buttonBK.bmp");
//   SDL_Texture *button_bk = SDL_CreateTextureFromSurface(app->renderer, button_bk_s);
//   SDL_FreeSurface(button_bk_s);
//   button_bk_s = SDL_LoadBMP("D:/Projects/C/MinGW_SDL2/resources/buttonMouseOnBK.bmp");
//   SDL_Texture *button_mouseon_bk = SDL_CreateTextureFromSurface(app->renderer, button_bk_s);
//   SDL_FreeSurface(button_bk_s);
//   button_bk_s = SDL_LoadBMP("D:/Projects/C/MinGW_SDL2/resources/buttonPressedBk.bmp");
//   SDL_Texture *button_pressed_bk = SDL_CreateTextureFromSurface(app->renderer, button_bk_s);
//   SDL_FreeSurface(button_bk_s);
//
//
//   SelectMenu *selectMenu = createSelectMenu(button_bk, button_mouseon_bk, button_pressed_bk);
//   add_item_to_selectMenu(selectMenu, MapFontUNICODEtexture(L"菜单1", cnfonts, 255,20,50,255), "menu1");
//   add_item_to_selectMenu(selectMenu, MapFontUNICODEtexture(L"菜单2", cnfonts, 155,200,50,255), "menu2");
//   add_item_to_selectMenu(selectMenu, MapFontUNICODEtexture(L"菜单3", cnfonts, 25,240,50,255), "menu3");

     Grid_init(1920, 1017);
     Sch_init();
}

static void destroy()
{
    FreeMapFont(cnfonts);
    SDL_DestroyTexture(texture);
    Sch_dispose();

    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
}

static void render()
{
    SDL_SetRenderDrawColor(app->renderer,bgColor.r,bgColor.g,bgColor.b,255);
    SDL_RenderClear(app->renderer);

    SDL_Color color = {255,0,0,255};
    SDL_Color verticesColor = {255,0,0,255};
    SDL_Vertex vertices[countOfPoints*2];
    int indices[(countOfPoints-1)*2*3];

    for (int i = 0; i < countOfPoints; i++) 
    {
            vertices[i*2].position.x = points_[i].x;
            vertices[i*2].position.y = points_[i].y;

            vertices[i*2].color = verticesColor;
            vertices[i*2+1].color = verticesColor;

            vertices[i*2+1].position.x = points[i].x;
            vertices[i*2+1].position.y = points[i].y;
    }
    for (int i = 0; i < countOfPoints*2-2; i++) {
        indices[i*3] = i;
        indices[i*3+1] = i+1;
        indices[i*3+2] = i+2;
    }
    //SDL_RenderGeometry(app->renderer, NULL, vertices, countOfPoints*2, indices, countOfPoints*6-6);     

    Grid_draw(app->renderer);
    GridEventProcess(app->window);

    SDL_Rect r;
    r.x = Grid_get_position().x;r.y = Grid_get_position().y;r.w = 420*Grid_get_distance();r.h = 290*Grid_get_distance();
    SDL_SetRenderDrawColor(app->renderer, 132, 0, 0, 230);
    SDL_RenderDrawRect(app->renderer, &r);
    r.x++;r.y++;r.w -=2;r.h -= 2;
    SDL_RenderDrawRect(app->renderer, &r);
    r.x++;r.y++;r.w -=2;r.h -= 2;
    SDL_RenderDrawRect(app->renderer, &r);
    r.x++;r.y++;r.w -=2;r.h -= 2;
    SDL_RenderDrawRect(app->renderer, &r);

    Sch_events_process();
    Sch_draw(app->renderer);

    Widget *widget;
    for (int i = 0; i < get_widget_num();i++)
    {
        get_one_widget(&widget);
        if (widget->widgetType == WIDGET_BUTTON)
        {
            renderBtFunction((Button *)widget, app->renderer);
        }
    }

    SDL_RenderPresent(app->renderer);
}

static void resize()
{
    int w = event.window.data1;
    int h = event.window.data2;
    SDL_Log("MainScreen Resized!, h = %d, w = %d", h, w);
}

void Main_screen_gameset(Game* g)
{
    g->create = create;
    g->render = render;
    g->destroy = destroy;
    g->resize = resize;
    app = g;
}




int GeometryDrawLine(int x1, int y1, int x2, int y2, SDL_Color color)
{
    SDL_Vertex vertex1,vertex2, vertex3, vertex4;

    vertex1.position.x = x1;
    vertex1.position.y = y1;

    vertex3.position.x = x2;
    vertex3.position.y = y2;

    if (abs(x2-x1) <= abs(y2-y1)) 
    {
        vertex2.position.x = x1+0.5;
        vertex2.position.y = y1;
        
        vertex4.position.x = x2+0.5;
        vertex4.position.y = y2;
    }
    else {
        vertex2.position.x = x1;
        vertex2.position.y = y1+0.5;

        vertex4.position.x = x2;
        vertex4.position.y = y2+0.5;
    }
    vertex1.color = color;
    vertex2.color = color;
    vertex3.color = color;
    vertex4.color = color;
    SDL_Vertex vertices[] = {vertex1, vertex2, vertex4, vertex1, vertex3, vertex4};
    SDL_RenderGeometry(app->renderer, NULL, vertices, 6, NULL, 0);
}
