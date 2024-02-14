#include<SDL.h>
#include "game.h"
static Game *app;

static void create()
{
    SDL_Log("SecondScreen: create()");

}
static void destroy()
{
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
}

static void render()
{
}

static void resize()
{
    int w = event.window.data1;
    int h = event.window.data2;
    SDL_Log("SecondScreen Resized!, h = %d, w = %d", h, w);
}

void Second_screen_gameset(Game* g)
{
    g->create = create;
    g->render = render;
    g->destroy = destroy;
    g->resize = resize;
    app = g;
}