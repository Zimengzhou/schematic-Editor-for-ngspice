#include <stdio.h>
#include <stdlib.h>

typedef struct Person       
{
    int age;
    int height;
}Person, *pPerson;


#define switch_case_start(switchWhat, caseWhat_1) \
switch (switchWhat)                               \
{                                                 \
    case caseWhat_1:                              
#define case_add(caseWhat)                        \
        break;                                    \
    case caseWhat:                                
#define switch_case_end                           \
        break;                                    \
}

typedef enum STATE
{
    STATE1 = 0,
    STATE2,
}STATE;

STATE state = STATE2;
                                                    
static int is_dot_in_linesegment(int point[2], int startpoint[2], int endpoint[2])
{
    int x = point[0], y = point[1];
    int x1 = startpoint[0], y1 = startpoint[1];
    int x2 = endpoint[0], y2 = endpoint[1];
    int result = 0;

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
int Compare(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}
int main(int argc, char **argv)                     
{   
    int* number = NULL;
    number = (int*)realloc(number, 10*sizeof(int));

    number[0] = 100;
    number[1] = 98;
    number[2] = 100;
    number[3] = 98;
    number[4] = 100;
    number[5] = 100;
    for (int i = 0; i<10; i++) 
    {
        printf("number[%d] = %d\n", i, number[i]);
        printf("free number\n");
    }
    free(number);
//    number = (int*)realloc(number, 10*sizeof(int));
//    if (number == NULL) 
//    {
//        printf("realloc faild!\n");
//    }

    return 0;
}


