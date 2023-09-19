#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "Stack.h"

int StackCtor(Steck* stk, const size_t Capacity)
{
    if(StackOK(stk))
        STACK_DUMP(stk);

    stk->Size = 0;
    stk->Capacity = Capacity;
    stk->data = (Elemt*) calloc(Capacity, sizeof(Elemt));

    if(StackOK(stk))
        STACK_DUMP(stk);

    return 0;
}

int StackPush(Steck* stk, Elemt value)
{
    if(StackOK(stk))
        STACK_DUMP(stk);

    if (stk->Size == stk->Capacity)
    {
        stk->Capacity *= 2;
        stk->data = (Elemt*) realloc(stk->data, stk->Capacity*sizeof(Elemt));
        for (int i = stk->Size; i < stk->Capacity; i++)
            stk->data[i] = 0;
    }

    stk->data[stk->Size++] = value;

    if(StackOK(stk))
        STACK_DUMP(stk);

    return 0;
}

int StackPop(Steck* stk, Elemt* refValue)
{
    if(StackOK(stk))
        STACK_DUMP(stk);

    if (2*stk->Size <= stk->Capacity)
    {
        stk->Capacity /= 2;
        free(stk->data+stk->Capacity);
    }

    *refValue = stk->data[--stk->Size];
    stk->data[stk->Size] = 0;

    if(StackOK(stk))
        STACK_DUMP(stk);
}

int StackDtor(Steck* stk)
{
    if(StackOK(stk))
        STACK_DUMP(stk);

    for (size_t i = 0; i < stk->Capacity; i++)
    {
        stk->data[i] = 0;
    }

    stk->Size = 0;
    stk->Capacity = 0;


    free(stk->data);
    stk->data = 0;

    stk = 0;

    return 0;
}

int StackOK(Steck* stk)
{
    if (stk == nullptr) return 1;
    if (stk->Size < 0) return 2;
    if (stk->Capacity < 0) return 3;
    if (stk->Capacity < stk->Size) return 4;
    for (int i = 0; i < stk->Size; i++)
        if (stk->data[i] == NAN || stk->data[i] == 0) return 5;
    return 0;
}

int StackDump(Steck* stk, const size_t nline, const char* namefile, const char* func)
{
    printf("Stack[%p] called from %s(%zd) %s\n", stk, namefile, nline, func);
    printf("  {\n   Size = %zd\n   Capacity = %zd\n   data[%p]\n"
    "\t{\n\t", stk->Size, stk->Capacity, stk->data);

    for (int i = 0; i < stk->Size; i++)
        printf("*[%d]=%d\n\t", i, stk->data[i]);

    for (int j = stk->Size; j < stk->Capacity; j++)
        printf(" [%d]=%d\n\t", j, stk->data[j]);

    printf("}\n  }\n");
    return 0;
}
