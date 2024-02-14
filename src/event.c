#include <SDL_events.h>
#include <SDL_mouse.h>
#include <SDL_stdinc.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"


KeyboardState keyboardstate;
Mouse mouse;

void Event_init()
{
    memset(&keyboardstate, 0, sizeof keyboardstate);

    //mouse
    memset(&mouse, 0, sizeof mouse);
    SDL_GetMouseState(&mouse.position.x, &mouse.position.y);
}

void Key_event_process(SDL_Scancode scancode, int down_up)
{
    keyboardstate.previous[scancode].isPressed = keyboardstate.current[scancode].isPressed; 
    keyboardstate.current[scancode].isPressed = down_up;
    //抬起按键时
    if (!down_up) 
    {
        if (keyboardstate.previous[scancode].isPressed)
        {
            keyboardstate.clicked[scancode].isPressed = 1;
        }
    }
}

//use in end of main loop, to clear the click key.
void Key_event_click_clear()
{
    //keyboard
    memset(&keyboardstate.clicked, 0, sizeof keyboardstate.clicked);

    //mouse
    memset(mouse.clicked, 0, 8*sizeof(int));
    memset(&mouse.motion, 0, sizeof mouse.motion);
    memset(&mouse.wheelchanges, 0, sizeof mouse.wheelchanges);
}

int Key_is_pressed(const int key)
{
    return keyboardstate.current[key].isPressed;
}

int Key_is_clicked(const int key)
{
    return keyboardstate.clicked[key].isPressed;
}

void Mouse_button_event_process(SDL_MouseButtonEvent button, int down_up)
{
    mouse.position.x = button.x;
    mouse.position.y = button.y;
    mouse.previous[button.button] = mouse.current[button.button];
    mouse.current[button.button] = down_up;
    if (!down_up) 
    {
        if (mouse.previous[button.button])
            mouse.clicked[button.button] = 1;
    }
}

void Mouse_motion_event_process(SDL_MouseMotionEvent motion)
{
    mouse.position.x = motion.x;
    mouse.position.y = motion.y;
    mouse.motion.x = motion.xrel;
    mouse.motion.y = motion.yrel;
}
void Mouse_wheel_event_process(SDL_MouseWheelEvent wheel)
{
    mouse.position.x = wheel.x;
    mouse.position.y = wheel.y;
    mouse.wheelchanges = wheel.preciseY;
}

int Mouse_is_pressed(const Uint32 button)
{
    return mouse.current[button];
}
int Mouse_is_clicked(const int button)
{
    return mouse.clicked[button];
}

int Mouse_is_moved()
{
    return mouse.motion.x || mouse.motion.y;
}
int Mouse_is_wheeled()
{
    return mouse.wheelchanges!=0;
}
void Mouse_get_position(int *x, int *y)
{

    *x = mouse.position.x; 
    *y = mouse.position.y;
}
void Mouse_get_motion(int *x, int *y)
{
    *x = mouse.motion.x; 
    *y = mouse.motion.y;
}
float Mouse_get_wheel()
{
    return mouse.wheelchanges;
}
