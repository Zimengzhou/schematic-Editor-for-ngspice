#include "Widget.h"

typedef struct ALL_Widget
{
    Widget *widget;
    struct ALL_Widget *next;

} ALL_Widget;

static ALL_Widget *head_all_widget;
static int widget_num = 0;

/*INIT　WIDGET  MODULE*/
void widget_init()
{
    head_all_widget = (ALL_Widget *)calloc(1, sizeof(ALL_Widget));
    head_all_widget->widget = NULL;
    head_all_widget->next = NULL;
}

void add_widget_to_allWidget(Widget *widget)
{
    ALL_Widget *head = head_all_widget;
    ALL_Widget *new = (ALL_Widget *)calloc(1, sizeof(ALL_Widget));
    new->widget = widget;
    new->next = NULL;
    if (head_all_widget->widget == NULL)
    {
        head_all_widget = new;
    }
    else{
        while (head->next != NULL)
        {
            head = head->next;
        }
        head->next = new;
    }
    widget_num++;
}

void get_one_widget(Widget **w)
{
    static ALL_Widget *all_widget = NULL;
    if (all_widget == NULL)
    {
        all_widget = head_all_widget;
    }
    *w = all_widget->widget;
    all_widget = all_widget->next;
}

int get_widget_num()
{
    return widget_num;
}