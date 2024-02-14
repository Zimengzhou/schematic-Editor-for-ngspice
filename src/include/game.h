#ifndef GLOBALLLLLLGAME
#define GLOBALLLLLLGAME

#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_video.h>
#include <SDL_log.h>
#include <SDL_events.h>
#ifdef __cplusplus
extern "C" {
#endif

/*每帧time*/
extern float deltaTime;

/*程序所在目录*/
extern char *appBasePath;
extern SDL_Event event;

typedef struct
{
  /**/
  SDL_Window *window;
  SDL_Renderer *renderer;

  char *title;

  void (*create)();   /*创建时执行一次*/
  void (*render)();   
  void (*destroy)();   /*销毁时执行*/
  void (*resize)();
}Game;

typedef enum
{
  GAME_CREATE = 0,
  GAME_RENDER,
  GAME_PAUSE
} GameFunTyp;

/* */
Game *createGame(const char *title, int x, int y, int w, int h, Uint32 flags,
                 void (*gameFunSet)(Game *g));

char *PATH(char *relativePath);
#ifdef __cplusplus
}
#endif
#endif
