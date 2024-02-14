#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Widget
{
    char name[100];
    int type;
} Widget;

typedef struct Button
{
    Widget widget;
    int isPressed;
    int x, y;
} Button;

void setButton(Button**bt)
{
    Button *button = (Button *)calloc(1, sizeof(Button));
    button->isPressed = 1;
    button->x = 20;
    button->y = 40;
    strcpy(button->widget.name, "seted Button");
    *bt = button;
}

int main(int argc, char const *argv[])
{
    Button *button = (Button *)calloc(1, sizeof(Button));
    button->isPressed = 1;
    button->x = 10;
    button->y = 10;
    strcpy(button->widget.name, "New Button");
    printf("Button.Widget.name : %s \n Button.isPressed: %d\n Button.x = %d, .y = %d\n", button->widget.name,button->isPressed,button->x,button->y);

    setButton(&button);
    printf("Button.Widget.name : %s \n Button.isPressed: %d\n Button.x = %d, .y = %d\n", button->widget.name,button->isPressed,button->x,button->y);

    return 0;
}
