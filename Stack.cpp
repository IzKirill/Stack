#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

int StackOK(Steck* stk)
{
    if (stk == NULL) return 1;
    if (stk->Size < 0) return 2;
    if (stk->Capacity < 0) return 3;
    if (stk->Capacity < stk->Size) return 4;
    for (int i = 0; i < stk->Size; i++)
        if (stk->data[i] == NAN || stk->data[i] == NULL) return 5;
    return 0;
}

int StackDump(Steck* stk, const size_t nline, const char* namefile, const char* func)
{
    printf("Stack[%p] called from %s(%zd) %s\n", stk, namefile, nline, func);
    printf("  {\n   Size = %zd\n   Capacity = %zd\n   data[%p]\n"
    "\t{\n\t", stk->Size, stk->Capacity, stk->data);

    for (int i = 0; i < stk->Size; i++)
        printf("*[%d]=%d\n\t", i, stk->data[i]);

    printf("}\n  }");
    return 0;
}
