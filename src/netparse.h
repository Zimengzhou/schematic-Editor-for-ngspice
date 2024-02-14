#ifndef NET_PARSE_H__
#define NET_PARSE_H__

#include "sch.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct Netconnection
{
    int *wireID;
    int wire_counts;
    int *junctionID;
    int junction_counts;
    char name[100];
}Netconnection;

//判断点是否在线段上，线段均为水平或竖直线


#ifdef __cplusplus
}
#endif
#endif

