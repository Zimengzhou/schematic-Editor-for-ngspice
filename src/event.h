#ifndef SZM_EVENT_H_
#define SZM_EVENT_H_
#include <SDL.h>
#include <SDL_events.h>

#ifdef __cplusplus
extern "C" {
#endif

void Event_init();
//Keyboard Events
typedef struct KeyPress
{
    int isPressed;
    SDL_Keycode keycode;

}KeyPress;

typedef struct
{
    KeyPress current[300];
    KeyPress previous[300];
    KeyPress clicked[300];
} KeyboardState;

void Key_event_click_clear();
void Key_event_process(SDL_Scancode scancode, int down_up);
int Key_is_pressed(const int key);
int Key_is_clicked(const int key);

//Mouse Events
typedef struct 
{
    int previous[8];
    int current[8];
    int clicked[8];
    struct
    {
        int x;
        int y;
    } position, motion;
    float wheelchanges;

} Mouse;

void Mouse_button_event_process(SDL_MouseButtonEvent button, int down_up);
void Mouse_motion_event_process(SDL_MouseMotionEvent motion);
void Mouse_wheel_event_process(SDL_MouseWheelEvent wheel);
int Mouse_is_clicked(const int button);
int Mouse_is_pressed(const Uint32 button);
int Mouse_is_moved();
int Mouse_is_wheeled();
void Mouse_get_motion(int *x, int *y);
void Mouse_get_position(int *x, int *y);
float Mouse_get_wheel();


#ifdef __cplusplus
}
#endif
#endif
