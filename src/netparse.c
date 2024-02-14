#include "netparse.h"
#include "sch.h"
#include <stdlib.h>
extern SchElements schElements;
        
static int is_dot_in_linesegment(int point[2], int startpoint[2], int endpoint[2]);
Netconnection *nets;

void Netparse_init()
{
    nets = (Netconnection*)calloc(sizeof(Netconnection), 1);
    nets->wireID = (int*)calloc(sizeof(int), 1);    
    nets->wire_counts = 0;
    nets->junctionID = (int*)calloc(sizeof(int), 1);    
    nets->junction_counts = 0;
}

static int is_dot_in_linesegment(int point[2], int startpoint[2], int endpoint[2])
{
    int x = point[0], y = point[1];
    int x1 = startpoint[0], y1 = startpoint[1];
    int x2 = endpoint[0], y2 = endpoint[1];

    //纵线
    if (x2 == x1) 
    {
        if (x == x1) 
        {
            if ((y>=y1&&y<=y2) || (y>=y2&&y<=y1)) 
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
            if ((x>=x1 && x<=x2) ||(x>=x2 && x<=x1))
                return 1;
            return 0;
        }
        return 0;
    }
    return 0;
}
