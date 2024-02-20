#include "sch.h"
#include "schgrid.h"
#include <string.h>

extern SchElements schElements;

static int dot_in_segment_not_start_and_end(int point[2], int startpoint[2], int endpoint[2]);

static void set_all_unselected();
void Sch_setElementsSelected()
{
    int cursor_on_no_item = 1;
    int curser[2];
    curser[0] = Grid_get_cursor().x;
    curser[1] = Grid_get_cursor().y;
    set_all_unselected();
    //Selected Wires
    for (int i = 0; i<schElements.wire_counts; i++) 
    {
        //光标在线上
        int startpoint[2], endpoint[2];
        startpoint[0] = schElements.wires[i].startpoint[0];
        startpoint[1] = schElements.wires[i].startpoint[1];
        endpoint[0] = schElements.wires[i].endpoint[0];
        endpoint[1] = schElements.wires[i].endpoint[1];

        if (dot_in_segment_not_start_and_end(curser, startpoint, endpoint)) 
        {
            schElements.selected_wires[i] = 1;
            cursor_on_no_item = 0;
            break;
        }
    }

    //Selected Junctions
    for (int i = 0; i < schElements.junction_counts; i++) 
    {
        if (curser[0] == schElements.junctions[i].x
            &&curser[1] == schElements.junctions[i].y)
        {
            schElements.selected_junctions[i] = 1;
            cursor_on_no_item = 0;
            break;
        }
    }
    cursor_on_no_item = 1;
}
static void set_all_unselected()
{
    memset(schElements.selected_wires, 0, sizeof(int)*schElements.wire_counts);     
    memset(schElements.selected_junctions, 0, sizeof(int)*schElements.junction_counts);
}

static int dot_in_segment_not_start_and_end(int point[2], int startpoint[2], int endpoint[2])
{
    int x = point[0], y = point[1];
    int x1 = startpoint[0], y1 = startpoint[1];
    int x2 = endpoint[0], y2 = endpoint[1];

    //纵线
    if (x2 == x1) 
    {
        if (x == x1) 
        {
            if ((y>y1&&y<y2) || (y>y2&&y<y1)) 
                return 1;
            return 0;
        }
        return 0;
    }
    //横线
    if (y2 == y1) 
    {
        if (y==y1) 
        {
            if ((x>x1 && x<x2) ||(x>x2 && x<x1))
                return 1;
            return 0;
        }
        return 0;
    }
    return 0;
}
