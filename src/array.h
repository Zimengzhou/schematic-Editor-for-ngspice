#pragma once

#include <stddef.h>
typedef struct 
{
    void* data;
    size_t elementSize;
    size_t length;
    size_t capacity;
} Array;

#define ArrayGet(array_p, index) (&((char*)(array_p)->data)[(index) * (array_p)->elementSize])

void ArrayNew(Array* a, size_t elementSize);
void ArrayAddIdx(Array *a, const size_t index, const void* elemValue);
void ArrayAdd(Array *a, const void* elemValue);
void ArrayRemove(Array *a, const size_t index);
void ArraySet(Array* a, const size_t index, const void* elemValue);
void ArrayFree(Array *a);
