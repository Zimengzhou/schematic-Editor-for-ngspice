#include "Widget.h"

static void topButtonClickedFunc(Button *button, void *data1, void *data2);
SelectMenu *createSelectMenu(SDL_Texture *button_bk, SDL_Texture *button_mouseon_bk, SDL_Texture *button_pressed_bk)
{
    SelectMenu *menu = (SelectMenu *)calloc(1, sizeof(SelectMenu));
    menu->topButton = NULL;
    menu->menuButtons.button = NULL;
    menu->menuButtons.next = NULL;
    menu->widget.child_visiable = 0;
    menu->widget.mouse_focused = 0;
    menu->widget.mouse_on = 0;
    menu->widget.parent = NULL;
    menu->widget.visiable = 0;
    menu->widget.widgetType = WIDGET_SELECTMENU;
    menu->widget.widgetRegion.x = 0;
    menu->widget.widgetRegion.y = 0;
    menu->expand = 0;
    menu->numbers = 0;

    menu->button_bk = button_bk;
    menu->button_mouseon_bk = button_mouseon_bk;
    menu->button_pressed_bk = button_pressed_bk;

    add_widget_to_allWidget((Widget *)menu);

    return menu;
}
static void showSelectMenuLists(SelectMenu *menu, int IsOrNot)
{
    struct buttonList *menuBts = &menu->menuButtons;
    if (menuBts->button==NULL)
        return;
    else
        showButton(menuBts->button, IsOrNot);
    int tempnum = 0;
    while (menuBts->next!=NULL)
    {
        menuBts = menuBts->next;
        tempnum++;
        showButton(menuBts->button, IsOrNot);
    }
    SDL_Log("Selected Menu item is %s", menu->widget.name);
}

static void topButtonClickedFunc(Button *button, void *data1, void *data2)
{
    SelectMenu* parentMenu = (SelectMenu* )button->widget.parent;
    parentMenu->expand = parentMenu->expand?0:1;
    showSelectMenuLists(parentMenu, parentMenu->expand);
}
static void buttonListButtonClickedFunc(Button *button, void *data1, void *data2)
{
    SelectMenu* parentMenu = (SelectMenu* )button->widget.parent;
    SDL_Rect topButton_rect = parentMenu->topButton->widget.widgetRegion;
    memcpy(parentMenu->topButton, button, sizeof(Button));
    setButtonRegion(parentMenu->topButton, topButton_rect);
    parentMenu->topButton->onClicked = topButtonClickedFunc;
    strcpy(parentMenu->widget.name, button->widget.name);
    parentMenu->expand = 0;
    showSelectMenuLists(parentMenu, 0);
}

void add_item_to_selectMenu(SelectMenu *menu, SDL_Texture *text, const char* buttonID)
{
    Button *firstBt = createButton3(text, menu->widget.widgetRegion.x, menu->widget.widgetRegion.y, menu->button_bk,menu->button_pressed_bk,menu->button_mouseon_bk);
    firstBt->widget.visiable = 0;
    setButtonID(firstBt, buttonID);
    menu->numbers = menu->numbers + 1;
    firstBt->widget.parent = (Widget*)menu;

    if (menu->topButton == NULL)
    {
        menu->topButton = (Button *)calloc(1, sizeof(Button));
        memcpy(menu->topButton, firstBt, sizeof(Button));
        showButton(menu->topButton, 1);
        setButtonRegion(menu->topButton, menu->topButton->widget.widgetRegion);
        add_widget_to_allWidget((Widget*)menu->topButton);
        menu->topButton->onClicked = topButtonClickedFunc;
    }
    firstBt->widget.widgetRegion.y += menu->numbers * firstBt->widget.widgetRegion.h;
    setButtonRegion(firstBt, firstBt->widget.widgetRegion);
    firstBt->onClicked = buttonListButtonClickedFunc;

    if(menu->menuButtons.button==NULL)
    {
        menu->menuButtons.button = firstBt;
    }
    else
    {
        struct buttonList *newList = (struct buttonList *)calloc(1, sizeof(struct buttonList));
        newList->button = firstBt;
        newList->next = NULL;

        struct buttonList *headList = &menu->menuButtons;
        while (headList->next != NULL)
        {
            headList = headList->next;
        }
        headList->next = newList;
    }
}

void setSelectMenu_topButton(SelectMenu *menu, Button *bt)
{
    menu->topButton = bt;
}
