#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "Stack.h"

error StackCtor(Stack* stk, const size_t Capacity, const char* name,
                const size_t birth_line, const char* birth_file, const char* birth_function)
{
    stk->stk_name = name;
    stk->birth_function = birth_function;
    stk->birth_line = birth_line;
    stk->birth_file = birth_file;

    if (Capacity <= 0)
    {
        printf("Incorrect value of Capacity.\n");
        return INCORRECTCAPACITY;
    }
    stk->Size = 0;
    stk->Capacity = Capacity;
    stk->data = (Elemt*) calloc(Capacity, sizeof(Elemt));

    if (!stk->data)
    {
        printf("Allocation failure.\n");
        return NODINMEMORY;
    }

    if(StackOK(stk))
        return STACK_DUMP(stk);

    return OK;
}

error StackPush (Stack* stk, Elemt value)
{
    if(StackOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (stk->Size == stk->Capacity)
    {
        stk->Capacity *= 2;
        stk->data = (Elemt*) realloc(stk->data, stk->Capacity*sizeof(Elemt));

        if (!stk->data)
        {
            printf("Allocation failure.\n");
            return NODINMEMORY;
        }

        for (size_t i = stk->Size; i < stk->Capacity; i++)
            stk->data[i] = 0;
    }

    stk->data[stk->Size++] = value;

    if(StackOK(stk) == ERROR)
        return STACK_DUMP(stk);

    return OK;
}

error StackPop (Stack* stk, Elemt* refValue)
{
    if(StackOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (stk->Size == 0)
    {
        printf("ERROR: stk->Size == 0\n");
        return STACK_DUMP(stk);
    }

    if (2*stk->Size <= stk->Capacity)
    {
        stk->Capacity /= 2;
    }

    *refValue = stk->data[--stk->Size];
    stk->data[stk->Size] = 0;

    if(StackOK(stk))
        return STACK_DUMP(stk);

    return OK;
}

error StackDtor (Stack* stk)  // double free>?
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

error StackOK (const Stack* stk)
{
    size_t n_error = 0;

    CHECKERROR(stk == nullptr);
    CHECKERROR(stk->Size < 0);
    CHECKERROR(stk->Capacity < 0);
    CHECKERROR(stk->Capacity < stk->Size);
    CHECKERROR(stk-> Capacity == 0);
    CHECKERROR(stk->data == NULL); // change of content?

    if (n_error != 0)
    {
        printf("Total errors - %d\n", n_error);
        return ERROR;
    }

    return OK;
}

error StackDump (Stack* stk, const size_t nline, const char* namefile, const char* func)
{   // -> to file
    // 100 % garanty
    printf("Stack[%p] %s from %s(%d) %s", stk, stk->stk_name, stk->birth_file,
                                          stk->birth_line, stk->birth_function);
    printf(" called from %s(%d) %s\n", namefile, nline, func);
    printf("  {\n   Size = %d\n   Capacity = %d\n   data[%p]\n"
    "\t{\n\t", stk->Size, stk->Capacity, (void*) stk->data);

    if (stk->Size > 0)
    {
        for (size_t i = 0; i < stk->Size; i++)
            printf("*[%d]=%d\n\t", i, stk->data[i]);

        if (stk->Capacity > 0)
        {
            for (size_t j = stk->Size; j < stk->Capacity; j++)
                printf(" [%d]=%d\n\t", j, stk->data[j]);
        }
    }

    printf("}\n  }\n");

    return ERROR;
}
