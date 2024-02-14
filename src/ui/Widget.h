#ifndef SZM_WIDGET_H
#define SZM_WIDGET_H

#ifdef __cplusplus
extern "C" {
#endif
#include <SDL.h>


enum WidgetType 
{
    WIDGET_BUTTON = 0,
    WIDGET_SELECTMENU,
    WIDGET_CONTAINER,
    WIDGET_TEXT_INPUT_AREA,
    WIDGET_SCOLL_AREA
};
typedef struct Widget Widget;
struct Widget
{
    char name[40];
    enum WidgetType widgetType;
    int visiable;
    int mouse_on;
    int mouse_focused;
    int child_visiable;
    SDL_Rect widgetRegion;
    /*widget 's parent*/
    Widget *parent;
};

void widget_init();
void add_widget_to_allWidget(Widget *widget);
void get_one_widget(Widget **w);
int get_widget_num();

typedef struct Button Button;
struct Button
{
    Widget widget;
    SDL_Texture *text;
    /*背景, 鼠标放上去时背景，点击时背景*/
    SDL_Texture *realBk;
    SDL_Texture *bk;
    SDL_Texture *bkOnClicked;
    SDL_Texture *bkOnFocused;
    /*点击执行函数*/
    void (*onClicked)(Button *button, void* data1, void* data2);
    void (*onPressed)(Button *button, void *data1, void *data2);
    void (*onReleased)(Button *button, void *data1, void *data2);

    SDL_Rect textRegion;
};
Button *createButton1(SDL_Texture *text);
Button *createButton2(SDL_Texture *text, int x, int y);
Button *createButton3(SDL_Texture *text, int x, int y, SDL_Texture *bk, SDL_Texture *bkOnClicked, SDL_Texture *bkOnFocused);
void renderBtFunction(Button *bt, SDL_Renderer *r);
void showButton(Button *bt, int isOrnot);
void setButtonText(Button *bt, SDL_Texture *text);
void setButtonBk(Button* bt, SDL_Texture *bk);
void setButtonBkOnClicked(Button* bt, SDL_Texture *bkOnClicked);
void setButtonBkOnFocused(Button* bt, SDL_Texture *bkOnFocused);
void setButtonRegion(Button *bt, SDL_Rect rect);
void setButtonID(Button *bt, const char *buttonid);

typedef struct selectMenu
{
    Widget widget;
    Button *topButton;
    struct buttonList
    {
        Button *button;
        struct buttonList *next;
    } menuButtons;
    /*if the to be selected item show*/
    int expand;
    int numbers;
    SDL_Texture *button_bk;
    SDL_Texture *button_pressed_bk;
    SDL_Texture *button_mouseon_bk;
} SelectMenu;
SelectMenu *createSelectMenu(SDL_Texture *button_bk, SDL_Texture *button_mouseon_bk, SDL_Texture *button_pressed_bk);
void add_item_to_selectMenu(SelectMenu *menu, SDL_Texture *text, const char *buttonID);
/*font.c*/
// typedef int* SZM_Text;
// SZM_Text SZM_CreateText(TTF_Font *font, const char *str, Game *game, int x, int y, Uint8 r, Uint8 g, Uint8 b);
// int SZM_SetText(SZM_Text textIndex, const char *str);
// void SZM_Font_dispose();
// void SZM_Font_Present();
// void SZM_GetSize(int *w, int *h, SZM_Text textIndex);
// int SZM_Font_Init();
#ifdef __cplusplus
}
#endif
#endif

