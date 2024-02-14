#include "Widget.h"
#include "mapfont.h"
static void defaultClickedFun(Button *button, void *data1, void *data2);
Button *createButton1(SDL_Texture *text)
{
    Button *button = createButton3(text, 0, 0, NULL, NULL, NULL);
    return button;
}

Button *createButton2(SDL_Texture *text, int x, int y)
{
    Button *button = createButton3(text, x, y, NULL, NULL, NULL);

    return button;
}

Button *createButton3(SDL_Texture *text, int x, int y, SDL_Texture *bk, SDL_Texture* bkOnClicked, SDL_Texture * bkOnFocused)
{
    Button *button = (Button *)calloc(1, sizeof(Button));
    button->widget.widgetType = WIDGET_BUTTON;
    button->bk = bk;
    button->bkOnClicked = bkOnClicked;
    button->bkOnFocused = bkOnFocused;
    button->text = text;
    SDL_Rect rect = {x, y, 0, 0};
    SDL_QueryTexture(bk, NULL, NULL, &rect.w, &rect.h);
    button->widget.widgetRegion = rect;
    SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h);
    button->textRegion = rect;
    button->widget.visiable = 0;
    button->widget.mouse_on = 0;
    button->widget.mouse_focused = 0;
    /*当前显示的背景*/
    button->realBk = button->bk;
    /*默认回调函数*/
    button->onClicked = defaultClickedFun;

    add_widget_to_allWidget((Widget*)button);

    return button;
}

void showButton(Button* bt, int isOrnot)
{
    bt->widget.visiable = isOrnot;
}

void renderBtFunction(Button* bt, SDL_Renderer* r)
{
    if (bt->widget.visiable == 0)
    {
        return;
    }
    
    SDL_RenderCopy(r, bt->realBk, NULL, &bt->widget.widgetRegion);
    SDL_RenderCopy(r, bt->text, NULL, &bt->textRegion);
}

static void defaultClickedFun(Button *button, void *data1, void *data2)
{
    SDL_Log("Button %s clicked!", button->widget.name);
}

/*button ID set*/
void setButtonID(Button* bt, const char* buttonid)
{
    strcpy(bt->widget.name, buttonid);
}

void setButtonText(Button* bt, SDL_Texture *text)
{
    bt->text = text;
}
void setButtonBk(Button* bt, SDL_Texture *bk)
{
    bt->bk = bk;
    bt->realBk = bk;
}
void setButtonBkOnClicked(Button* bt, SDL_Texture *bkOnClicked)
{
    bt->bkOnClicked = bkOnClicked;
}
void setButtonBkOnFocused(Button* bt, SDL_Texture *bkOnFocused)
{
    bt->bkOnFocused = bkOnFocused;
}
void setButtonRegion(Button* bt, SDL_Rect rect)
{
    bt->widget.widgetRegion = rect;
    bt->textRegion.x = rect.x + rect.w/2 - bt->textRegion.w/2;
    bt->textRegion.y = rect.y + rect.h/2 - bt->textRegion.h/2;
}