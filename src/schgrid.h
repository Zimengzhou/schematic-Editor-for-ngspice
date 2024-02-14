#ifndef SCH_GRID__H__
#define SCH_GRID__H__

#include <SDL.h>
#ifdef __cplusplus
extern "C" {
#endif

#define GRID_INITIAL_DIS 12
#define GRID_X_MAX 5000
#define GRID_Y_MAX 5000

typedef struct GridConf
{
    float scale;
    
    SDL_FPoint position;

    SDL_Color color;

    float width;

    SDL_Rect drawArea;

    struct cursor{
        int x;
        int y;
    }cursor;

}GridConf;

void Grid_init(int windowWidth, int WindowHeight);
void Grid_draw(SDL_Renderer* renderer);
void GridScale(float deltaScale, SDL_Window* w);

void Grid_set_cursor(int mouseX, int mouseY);
void Grid_set_position(float x, float y);
void Grid_move_position(float x, float y);

float Grid_get_distance();
SDL_FPoint Grid_get_position();
struct cursor Grid_get_cursor();


void GridEventProcess(SDL_Window *w);



#ifdef __cplusplus
}
#endif

#endif
