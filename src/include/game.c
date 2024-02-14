#include "game.h"
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_scancode.h>
#include <stdlib.h>
#include <string.h>

Game *createGame(const char *title, int x, int y, int w, int h, Uint32 flags,
                void (*gameFunSet)(Game *g))
{
    Game *app = (Game*)calloc(1, sizeof(Game));
    
    app->title = (char*)calloc(100, sizeof(char));
    strcpy(app->title, title);

    app->window = SDL_CreateWindow(title, x, y, w, h, flags);
//                  0 :  direct3d
//                  1 :  direct3d11
//                  2 :  direct3d12
//                  3 :  opengl
//                  4 :  opengles2
//                  5 :  software
    app->renderer = SDL_CreateRenderer(app->window, 3, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);

    gameFunSet(app);
    app->create();

    return app;
}

char *PATH(char* relativePath)
{
    char *path = (char *)calloc(strlen(appBasePath)+strlen(relativePath)+2,sizeof(char));
    strcat(path,appBasePath);
    strcat(path, relativePath);
    return path;
}


