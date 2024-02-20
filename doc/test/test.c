#include <stdio.h>
#include "array.h"


typedef struct
{
    //起始点
    int startpoint[0];
    int x0;
    int y0;
    int endpoint[0];
    int x1;
    int y1;
    float width;
    int netID;

}Wire;

Array wires;


int main(int argc, char const *argv[])
{
    printf("Hello World!\n");
    ArrayNew(&wires, sizeof(Wire));
    Wire w = {
        .x0 = 10,
        .x1 = 11,
        .y0 = 12,
        .y1 = 13,
        .width = 5,
        .netID = 2
    };
    Wire w1 = {
        .x0 = 20,
        .x1 = 21,
        .y0 = 22,
        .y1 = 23,
        .width = 5,
        .netID = 2
    };
    ArrayAdd(&wires, &w);
    ArrayAdd(&wires, &w1);

    printf("wires Array has %zu elements.\n", wires.length);
    printf("wires [0] is\nx0 = %d.\n""y0 = %d\n""x1 = %d\ny1 = %d\n", ((Wire*)ArrayGet(&wires, 0))->x0,((Wire*)ArrayGet(&wires, 0))->y0,((Wire*)ArrayGet(&wires, 0))->x1,((Wire*)ArrayGet(&wires, 0))->y1);
    printf("wires [1] is\nx0 = %d.\n""y0 = %d\n""x1 = %d\ny1 = %d\n", ((Wire*)ArrayGet(&wires, 1))->x0,((Wire*)ArrayGet(&wires, 1))->y0,((Wire*)ArrayGet(&wires, 1))->x1,((Wire*)ArrayGet(&wires, 1))->y1);

    printf("remove wires[0]\n");
    ArrayRemove(&wires, 0);
    printf("wires [0] is\nx0 = %d.\n""y0 = %d\n""x1 = %d\ny1 = %d\n", ((Wire*)ArrayGet(&wires, 0))->x0,((Wire*)ArrayGet(&wires, 0))->y0,((Wire*)ArrayGet(&wires, 0))->x1,((Wire*)ArrayGet(&wires, 0))->y1);

    printf("insert w at index 0");
    ArrayAddIdx(&wires, 0, &w);
    printf("wires [0] is\nx0 = %d.\n""y0 = %d\n""x1 = %d\ny1 = %d\n", ((Wire*)ArrayGet(&wires, 0))->x0,((Wire*)ArrayGet(&wires, 0))->y0,((Wire*)ArrayGet(&wires, 0))->x1,((Wire*)ArrayGet(&wires, 0))->y1);
    printf("wires [1] is\nx0 = %d.\n""y0 = %d\n""x1 = %d\ny1 = %d\n", ((Wire*)ArrayGet(&wires, 1))->x0,((Wire*)ArrayGet(&wires, 1))->y0,((Wire*)ArrayGet(&wires, 1))->x1,((Wire*)ArrayGet(&wires, 1))->y1);
    return 0;

}
