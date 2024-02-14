#ifndef SCH_____H__
#define SCH_____H__

#include <SDL.h>
#include <SDL_pixels.h>
#ifdef __cplusplus
extern "C" {
#endif

#define VERSION_SCH "1.0"

//结点
typedef struct Junction
{
    int x,y;
    float width;
    SDL_Color color;
}Junction;

typedef struct Wire
{
    //起始点
    int startpoint[0];
    int x0;
    int y0;
    int endpoint[0];
    int x1;
    int y1;
    int *divide_points, divide_counts;
    float width;
    SDL_Color color;
    int netID;
}Wire;


typedef struct SchElements
{
    Wire* wires;
    int wire_counts;
    Junction* junctions;
    int junction_counts;
}SchElements;

void Sch_init();
void Sch_draw(SDL_Renderer*renderer);
void Sch_events_process();
void Sch_createWire(int x, int y);
void Sch_draw_wire();
void Sch_dispose();

#define switch_case_start(switchWhat, caseWhat_1) \
switch (switchWhat)                               \
{                                                 \
    case caseWhat_1:                              
#define case_add(caseWhat)                        \
        break;                                    \
    case caseWhat:                                
#define switch_case_end                           \
        break;                                    \
}

#ifdef __cplusplus
}
#endif

#endif
