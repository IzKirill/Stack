#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "Stack.h"

int main ()
{
    Stack stk1 = { };
    printf("1\n");
    STACK_CTOR(&stk1, 10);
    StackPush(&stk1, 2);
    STACK_DUMP(&stk1);

    StackDtor(&stk1);
}

error StackCtor(Stack* stk, const size_t Capacity, const char* name,
                const size_t birth_line, const char* birth_file, const char* birth_function)
{
    if (stk == NULL)
    {
        printf("StackCtor: Incorrect value of adress stack massive == NULL");
        return NULLSTRSTK;
    }

    if (Capacity <= 0)
    {
        printf("StackCtor: Incorrect value of Capacity - %d.\n", Capacity);
        return INCORRECTCAPACITY;
    }

    stk->stk_name = name;
    stk->birth_function = birth_function;
    stk->birth_line = birth_line;
    stk->birth_file = birth_file;

    stk->Size = 0;
    stk->Capacity = Capacity;
    stk->data = (Elemt*) calloc(Capacity, sizeof(Elemt));

    if (!stk->data)
    {
        printf("StackCtor: Allocation failure.\n");
        return NODINMEMORY;
    }

    if(STACKOK(stk))
        return STACK_DUMP(stk);

    return OK;
}

error StackPush (Stack* stk, Elemt value)
{
    if(STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (stk->Size == stk->Capacity)
    {
        stk->Capacity *= 2;
        stk->data = (Elemt*) realloc(stk->data, stk->Capacity*sizeof(Elemt));

        if (!stk->data)
        {
            printf("StackPush: Allocation failure.\n");
            return NODINMEMORY;
        }

        for (size_t nelemnt = stk->Size; nelemnt < stk->Capacity; nelemnt++)
            stk->data[nelemnt] = 0;
    }

    stk->data[stk->Size++] = value;

    if(STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    return OK;
}

error StackPop (Stack* stk, Elemt* refValue)
{
    if(STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (refValue == NULL)
    {
        printf("StackPop: Incorrect adress of refValue == NULL");
        return ADRESSNULL;
    }

    if (stk->Size == 0)
    {
        printf("StackPop: ERROR: stk->Size == 0\n");
        return SIZEEQUALZERO;
    }

    if (2*stk->Size <= stk->Capacity)
    {
        stk->Capacity /= 2;
    }

    *refValue = stk->data[--stk->Size];
    stk->data[stk->Size] = 0;

    if(STACKOK(stk))
        return STACK_DUMP(stk);

    return OK;
}

error StackDtor (Stack* stk)  // double free>?
{
    if(STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    for (size_t nelemnt = 0; nelemnt < stk->Capacity; nelemnt++)
    {
        stk->data[nelemnt] = 0;
    }

    stk->Size = 0;
    stk->Capacity = 0;


    free(stk->data);
    stk->data = 0;

    stk = 0;

    return OK;
}

error StackOK(const Stack* stk, const size_t line,
              const char* namefile, const char* func)
{
    size_t n_error = 0;

    if (stk == nullptr)
    {
        printf("%s:%d: error in function %s: "
               "Stack pointer equal zero.\n", namefile, line, func);
        n_error++;
    }
    if (stk->Size < 0)
    {
        printf("%s:%d: error in function %s: "
               "Stack size less zero and equal %d\n", namefile, line, func, stk->Size);
        n_error++;
    }
    if (stk->Capacity < 0)
    {
        printf("%s:%d: error in function %s: "
               "Stack capacity less zero and equal %d\n", namefile, line, func, stk->Capacity);
        n_error++;
    }
    if (stk->Capacity < stk->Size)
    {
        printf("%s:%d: error in function %s: Stack capacity less stack size: "
                "capacity = %d, size = %d\n", namefile, line, func , stk->Capacity, stk->Size);
        n_error++;
    }
    if (stk-> Capacity == 0)
    {
        printf("%s:%d: error in function %s: "
               "Stack capacity equal zero.\n", namefile, line, func);
    }
    if (stk->data == NULL)
    {
        printf("%s:%d: error in function %s: "
               "In the stack adress of massive equal zero.\n", namefile, line, func);
        n_error++;
    }

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
        for (size_t nelemnt = 0; nelemnt < stk->Size; nelemnt++)
            printf("*[%d]=%d\n\t", nelemnt, stk->data[nelemnt]);

        if (stk->Capacity > 0)
        {
            for (size_t nelemnt = stk->Size; nelemnt < stk->Capacity; nelemnt++)
                printf(" [%d]=%d\n\t", nelemnt, stk->data[nelemnt]);
        }
    }

    printf("}\n  }\n");

    return ERROR;
}
