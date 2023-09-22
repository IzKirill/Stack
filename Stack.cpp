#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "Stack.h"

int StackCtor(Steck* stk, const size_t Capacity)
{
    stk->Size = 0;
    stk->Capacity = Capacity;
    stk->data = (Elemt*) calloc(Capacity, sizeof(Elemt));

    if(StackOK(stk))
        return STACK_DUMP(stk);

    return OK;
}

int StackPush(Steck* stk, Elemt value)
{
    if(StackOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (stk->Size == stk->Capacity)
    {
        stk->Capacity *= 2;
        stk->data = (Elemt*) realloc(stk->data, stk->Capacity*sizeof(Elemt));
        for (int i = stk->Size; i < stk->Capacity; i++)
            stk->data[i] = 0;
    }

    stk->data[stk->Size++] = value;

    if(StackOK(stk) == ERROR)
        return STACK_DUMP(stk);

    return OK;
}

int StackPop(Steck* stk, Elemt* refValue)
{
    if(StackOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (2*stk->Size <= stk->Capacity)
    {
        stk->Capacity /= 2;
        free(stk->data+stk->Capacity);
    }

    *refValue = stk->data[--stk->Size];
    stk->data[stk->Size] = 0;

    if(StackOK(stk))
        return STACK_DUMP(stk);
}

int StackDtor(Steck* stk)
{
    if(StackOK(stk) == ERROR)
        return STACK_DUMP(stk);

    for (size_t i = 0; i < stk->Capacity; i++)
    {
        stk->data[i] = 0;
    }

    stk->Size = 0;
    stk->Capacity = 0;


    free(stk->data);
    stk->data = 0;

    stk = 0;

    return OK;
}

int StackOK(const Steck* stk)
{
    size_t n_error = 0;

    checkerror(stk == nullptr);
    checkerror(stk->Size < 0);
    checkerror(stk->Capacity < 0);
    checkerror(stk->Capacity < stk->Size);
    checkerror(stk-> Capacity == 0);
    checkerror(stk->data == NULL);

    if (n_error != 0)
    {
        printf("Total errors - %d\n", n_error);
        return ERROR;
    }

    return OK;
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

    return ERROR;
}
