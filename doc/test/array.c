#include "array.h"
#include <corecrt.h>
#include <stdlib.h>
#include <string.h>

void ArrayNew(Array* a, size_t elementSize)
{
    a->data = NULL;
    a->elementSize = elementSize;
    a->length = 0;
    a->capacity = 0;
}

static void ExpandCap(Array* a)
{
    if (a->length == a->capacity)
    {
        const size_t capacity = a->capacity==0 ? 1:a->capacity*2;
        a->capacity = capacity;
        const size_t size = a->capacity * a->elementSize;
        a->data = realloc(a->data, size);   
    }
}

void ArrayAddIdx(Array *a, const size_t index, const void* elemValue)
{
    ExpandCap(a);   
    a->length++;
    memmove(ArrayGet(a, index+1), ArrayGet(a, index), a->elementSize*(a->length-index-1));
    memcpy(ArrayGet(a, index), elemValue, a->elementSize);
}

void ArrayAdd(Array *a, const void* elemValue)
{
    ExpandCap(a);
    a->length++;
    memcpy(ArrayGet(a, a->length-1), elemValue, a->elementSize);
}

void ArrayRemove(Array *a, const size_t index)
{
    if (index+1 == a->length)
        a->length--;
    else
    {
        memmove(ArrayGet(a, index), ArrayGet(a, index+1), a->elementSize*(a->length-index-1));
        a->length--;
    }
}

void ArraySet(Array* a, const size_t index, const void* elemValue)
{
    memcpy(ArrayGet(a, index), elemValue, a->elementSize);
}

void ArrayFree(Array *a)
{
    if (a->data)
    {
        free(a->data);
    }
    memset(a, 0, sizeof *a);
}
