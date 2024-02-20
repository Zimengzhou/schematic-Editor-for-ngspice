#include <SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL_image.h>
//#include <SDL_ttf.h>
#include <stdlib.h>
#include <locale.h>

#include "game.h"
#include "Widget.h"
#include "event.h"

uint32_t currentTime;
float deltaTime;
char *appBasePath;
SDL_bool isQuit = SDL_FALSE;
SDL_bool hasEvent = SDL_TRUE;
SDL_Event event;

#define MainWinW 1900
#define MainWinH 1000
#define MainWinMaxW 5000
#define MainWinMaxH 1040

void Main_screen_gameset(Game *g);

//窗口属性
// SDL_HitTestResult dragWindow(SDL_Window* win,const SDL_Point* area,void* data);

/*SDL初始化*/
void init()
{
    int res = 0;
    /* 初始化所有组件*/
    res = SDL_Init(SDL_INIT_EVERYTHING);
    if (res != 0)
    {
        SDL_Log(SDL_GetError());
    }
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    //res = TTF_Init();
    res = 0;
    if (res!= 0)
    {
        SDL_Log(SDL_GetError());
    }

    /*程序所在的目录*/
    appBasePath = SDL_GetBasePath();
    widget_init();
    //key_event_init
    Event_init();
}

int main(int argc,char** argv)
{
    init();
    
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
//    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
//    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_ERROR);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    /*一个Game即一个窗口*/
    Game *app = createGame("szm schematic editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            MainWinW, MainWinH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE,
            Main_screen_gameset);
    // SDL_SetWindowHitTest(app->window, dragWindow, NULL);
    while (!isQuit) 
    {
        currentTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) 
        {
            hasEvent = SDL_TRUE;
            if (event.type == SDL_QUIT) 
            {
                isQuit = SDL_TRUE;
            }

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                        SDL_Log("Windows ID = %d", event.window.windowID);
                }
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    Uint32 app1WindowID = SDL_GetWindowID(app->window);
                    if (event.window.windowID == app1WindowID)
                    {
                        app->resize();
                    }
                }
            }

            /*When Mouse moved*/
            if (event.type == SDL_MOUSEMOTION)
            {
                Mouse_motion_event_process(event.motion);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                Mouse_button_event_process(event.button, 1);
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                Mouse_button_event_process(event.button, 0);
            }
            if (event.type == SDL_MOUSEWHEEL) 
            {
                Mouse_wheel_event_process(event.wheel);
            }
            
            else if(event.type == SDL_KEYDOWN)
            {
                Key_event_process(event.key.keysym.scancode, 1);
                if (event.key.keysym.scancode==SDL_SCANCODE_Q) 
                {
                    isQuit = SDL_TRUE;
                }
                /*F2截图*/
                if (event.key.keysym.scancode==SDL_SCANCODE_F2)
                {
                    SDL_Surface *screenshot = SDL_CreateRGBSurfaceWithFormat(0, 1200, 800, 32, SDL_PIXELFORMAT_RGBA8888);
                    SDL_RenderReadPixels(app->renderer, NULL, SDL_PIXELFORMAT_RGBA8888, screenshot->pixels, screenshot->pitch);
                    IMG_SaveJPG(screenshot, PATH("savedata/screenshot.jpg"),100);
                    SDL_FreeSurface(screenshot);
                    SDL_Log("ScreenShot took");
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                Key_event_process(event.key.keysym.scancode, 0);
            }
        }
        if (hasEvent) 
        {
            app->render();
            hasEvent = SDL_FALSE;
        }

        Key_event_click_clear();

        int fpstimeshould = 20 - SDL_GetTicks() + currentTime;
        if (fpstimeshould > 0)
        {
            SDL_Delay(fpstimeshould);
        }
        else
            SDL_Delay(20);
        deltaTime = (SDL_GetTicks()- currentTime)/1000.0;
    }
    /*销毁窗口、renderer、Texture、退出*/
    app->destroy();
    free(app);
    IMG_Quit();
    //TTF_Quit();
    SDL_Quit();
    
    return 0;
}

SDL_HitTestResult dragWindow(SDL_Window* win,const SDL_Point* area,void* data)
{

    int w,h;
    SDL_GetWindowSize(win,&w,&h);

    if (area->x < 5) 
    {
        return SDL_HITTEST_RESIZE_LEFT;
    }
    else if(area->x > w-5)
    {
        return SDL_HITTEST_RESIZE_RIGHT;
    }
    else if(area->y < 5)
    {
        return SDL_HITTEST_RESIZE_TOP;
    }
    else if(area->y > h-5)
    {
        return SDL_HITTEST_RESIZE_BOTTOM;
    }
    else if (area->x > 0 && area->x < w && area->y > 0 && area->y < 28) 
    {
        return SDL_HITTEST_DRAGGABLE;
    }


    return SDL_HITTEST_NORMAL;
}




/**
            if (event.type == SDL_MOUSEMOTION)
            {
                SDL_Point mousePos;
                SDL_GetMouseState(&mousePos.x, &mousePos.y);

                Widget *widget;
                for (int i = 0; i < get_widget_num();i++)
                {
                    get_one_widget(&widget);
                    if (SDL_PointInRect(&mousePos, &widget->widgetRegion))
                    {
                        widget->mouse_on = 1;

                        if (widget->widgetType == WIDGET_BUTTON)
                        {
                            Button *bt = (Button *)widget;
                            bt->realBk = bt->bkOnFocused;
                            // SDL_Log("Button have a mouse within!_____");
                        }
                    }
                    else
                    {
                        widget->mouse_on = 0;
                        widget->mouse_focused = 0;
                        if (widget->widgetType == WIDGET_BUTTON)
                        {
                            Button *bt = (Button *)widget;
                            bt->realBk = bt->bk;
                        }
                    }
                }

                //Grid move
                if (rightMouseButtondown) 
                    Grid_move_position(event.motion.xrel, event.motion.yrel);
                else
                {
                    Grid_set_cursor(mousePos.x, mousePos.y);
                    if (Sch_get_draw_wire_state() == DRAW_WIRE_DRAWING) 
                    {
                        Sch_draw_wire();
                    }
                }
            }
            //When Mouse down or released.
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                Mouse_button_event_process(event.button, 1);
                //SDL_Log("the mouse %d is pressed or released  %d",event.button.button, event.button.state);
                if (event.button.button == 3) 
                {
                    rightMouseButtondown = SDL_TRUE;
                }

                Widget *widget;
                for (int i = 0; i < get_widget_num();i++)
                {
                    get_one_widget(&widget);
                    if (widget->mouse_on)
                    {
                        widget->mouse_focused = 1;
                        if (widget->widgetType == WIDGET_BUTTON)
                        {
                            Button *bt = (Button *)widget;
                            bt->realBk = bt->bkOnClicked;
                        }
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                Mouse_button_event_process(event.button, 0);
                Widget *widget;
                for (int i = 0; i < get_widget_num();i++)
                {
                    get_one_widget(&widget);
                    if (widget->mouse_focused && widget->visiable)
                    {
                        widget->mouse_focused = 0;
                        if (widget->widgetType == WIDGET_BUTTON)
                        {
                            Button *bt = (Button *)widget;
                            bt->realBk = widget->mouse_on?bt->bkOnFocused:bt->bk;
                            bt->onClicked(bt,NULL,NULL);
                        }
                    }
                }
                if (event.button.button == 3) {
                    rightMouseButtondown = SDL_FALSE;
                }

                if ((event.button.button == 1)&&(Sch_get_draw_wire_state()==DRAW_WIRE_DRAWING)) 
                {
                    Sch_draw_wire_end();
                }
            }
            if (event.type == SDL_MOUSEWHEEL) 
            {
                if (event.wheel.direction == SDL_MOUSEWHEEL_NORMAL) {
                    GridScale(event.wheel.preciseY, app->window);
                }
            }

*/
