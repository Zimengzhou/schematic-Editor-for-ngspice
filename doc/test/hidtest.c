#include <stdio.h>
#include <stdlib.h>

struct persion
{
    int sanwei[0];
    int x;
    int y;
    int t;
};

int main(int argc, char const *argv[])
{
    struct persion *P = (struct persion*)calloc(sizeof(struct persion), 10000);
    struct persion *P_1 = P;
    for (int i = 0; i < 10000; i++)
    {
        P->x = i;
        P->y = i;
        P->t = i;
        if (i<9999) 
        {
            P++;
        }
    }
    P = P_1;

    for (int i = 0; i < 10000; i++)
    {
        printf("P[%d].x = %d\n",i,P->x);
        if (i<9999) 
        {
            P++;
        }
    }
    return 0;
}
