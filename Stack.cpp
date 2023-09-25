#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <stdint.h>
#include "Stack.h"

error StackCtor(Stack* stk, const size_t Capacity, const char* name,
                const size_t birth_line, const char* birth_file, const char* birth_function)
{
    if (stk == NULL)
    {
        printf("ERROR: StackCtor: Incorrect value of adress stack massive == NULL");
        return NULLSTRSTK;
    }

    if (Capacity <= 0)
    {
        printf("ERROR: StackCtor: Stack Incorrect value of Capacity - %d.\n", Capacity);
        return INCORRECTCAPACITY;
    }

    AddCanary(stk);

    stk->isStackCtor = 1;

    stk->stk_name = name;
    stk->birth_function = birth_function;
    stk->birth_line = birth_line;
    stk->birth_file = birth_file;

    stk->Size = 0;
    stk->Capacity = Capacity;
    stk->data = (Elemt*) (calloc(2*sizeof(CanaryType)+Capacity*sizeof(Elemt), sizeof(char))
                          +sizeof(CanaryType));
    ((CanaryType*) stk->data)[-1] = left_canary;
    *(CanaryType*) ((stk->data)+stk->Capacity*sizeof(Elemt)) = right_canary;

    AddHash(stk);

    if (!stk->data)
    {
        printf("ERROR: StackCtor: Stack(%s) Allocation failure.\n",
                stk->stk_name);
        return NODINMEMORY;
    }

    if(STACKOK(stk))
        return STACK_DUMP(stk);

    return OK;
}

error StackPush (Stack* stk, Elemt value)
{
    if (stk == NULL)
    {
        printf("ERROR: StackPush: Stack == NULL\n");
        return NULLSTRSTK;
    }

    if (CheckCanary(stk) == ATTACKCANARY)
    {
        STACK_DUMP(stk);
        return ATTACKCANARY;
    }

    if (!stk->isStackCtor)
    {
        printf("ERROR: StackPush: Stack does not created. Use funcion StackCtor first.\n");
        return STACKNOTCTOR;
    }

    if (STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (stk->Size == stk->Capacity)
    {
        stk->Capacity *= 2;
        stk->data = (Elemt*) realloc(stk->data - sizeof(CanaryType),
                                  stk->Capacity*sizeof(Elemt)+2*sizeof(CanaryType));

        if (!stk->data)
        {
            printf("ERROR: StackPush: Stack(%s) Allocation failure.\n",
                    stk->stk_name);
            return NODINMEMORY;
        }

        for (size_t nelemnt = stk->Size; nelemnt < stk->Capacity; nelemnt++)
            stk->data[nelemnt] = 0;
    }

    stk->data[stk->Size++] = value;

    ChangeHash(stk);

    if(STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    return OK;
}

error StackPop (Stack* stk, Elemt* refValue)
{
    if (stk == NULL)
    {
        printf("ERROR: StackPop: Stack == NULL\n");
        return NULLSTRSTK;
    }

    if (CheckCanary(stk) == ATTACKCANARY)
    {
        STACK_DUMP(stk);
        return ATTACKCANARY;
    }

    if (!stk->isStackCtor)
    {
        printf("ERROR: StackPop: Stack does not created. Use funcion StackCtor first.\n");
        return STACKNOTCTOR;
    }

    if(STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    if (refValue == NULL)
    {
        printf("ERROR: StackPop: Stack(%s) Incorrect adress of refValue == NULL",
                stk->stk_name);
        return ADRESSNULL;
    }

    if (stk->Size == 0)
    {
        printf("ERROR: StackPop: ERROR: Stack(%s) Size == 0\n", stk->stk_name);
        return SIZEEQUALZERO;
    }

    if (2*stk->Size <= stk->Capacity)
    {
        stk->Capacity /= 2;
    }

    *refValue = stk->data[--stk->Size];
    stk->data[stk->Size] = 0;

    ChangeHash(stk);

    if(STACKOK(stk))
        return STACK_DUMP(stk);

    return OK;
}

error StackDtor (Stack* stk)
{
    if (stk == NULL)
    {
        printf("ERROR: StackDtor: Stack == NULL\n");
        return NULLSTRSTK;
    }

    if (CheckCanary(stk) == ATTACKCANARY)
    {
        STACK_DUMP(stk);
        return ATTACKCANARY;
    }

    if (!stk->isStackCtor)
    {
        printf("ERROR: StackDtor: Stack does not created. Use funcion StackCtor first.\n");
        return STACKNOTCTOR;
    }

    if (stk->isStackDtor)
    {
        printf("ERROR: StackDtor: Stack(%s) is already deleted.\n", stk->stk_name);
        return STACKDTOR;
    }

    if(STACKOK(stk) == ERROR)
        return STACK_DUMP(stk);

    for (size_t nelemnt = 0; nelemnt < stk->Capacity; nelemnt++)
    {
        stk->data[nelemnt] = 0;
    }

    stk->Size = 0;
    stk->Capacity = 0;


    free((char*)stk->data-sizeof(CanaryType));
    stk->data = 0;
    stk->StkHash = 0;

    stk->isStackDtor = 1;
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
               "Stack pointer equal zero.\n", namefile, line,
                                             func);
        n_error++;
        return ERROR;
    }
    if (stk->Size < 0)
    {
        printf("%s:%d: error in function %s: "
               "Stack(%s) size less zero and equal %d\n", namefile, line,
                                      func, stk->stk_name, stk->Size);
        n_error++;
    }
    if (stk->Capacity < 0)
    {
        printf("%s:%d: error in function %s: "
               "Stack(%s) capacity less zero and equal %d\n", namefile, line,
                                      func, stk->stk_name, stk->Capacity);
        n_error++;
    }
    if (stk->Capacity < stk->Size)
    {
        printf("%s:%d: error in function %s: Stack(%s) capacity less stack size: "
                "capacity = %d, size = %d\n", namefile, line, func, stk->stk_name,
                                            stk->Capacity, stk->Size);
        n_error++;
    }
    if (stk-> Capacity == 0)
    {
        printf("%s:%d: error in function %s: "
               "Stack(%s) capacity equal zero.\n", namefile, line,
                                                func, stk->stk_name);
    }
    if (stk->data == NULL)
    {
        printf("%s:%d: error in function %s: "
               "In the stack(%s) adress of massive equal zero.\n", namefile, line,
                                                                func, stk->stk_name);
        n_error++;
    }

    if (n_error != 0)
    {
        printf("Total errors - %d\n", n_error);
        return ERROR;
    }

    return OK;
}

#ifndef RELEASE
error StackDump (Stack* stk, const size_t nline, const char* namefile, const char* func)
{   // -> to file
    // 100 % garanty
    if (stk == NULL)
    {
        printf("ERROR: Stack == NULL\n");
        return NULLSTRSTK;
    }

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

error AddCanary(Stack* stk)
{
    if (stk == NULL)
    {
        printf("ERROR: AddCanary: Stack == NULL\n");
        return NULLSTRSTK;
    }

    stk->left_canary = left_canary;
    stk->right_canary = right_canary;

    return OK;
}

error CheckCanary(Stack* stk)
{
    if (stk == NULL)
    {
        printf("ERROR: CheckCanary: Stack == NULL\n");
        return NULLSTRSTK;
    }

    size_t nattacks = 0;

    if (stk->left_canary != left_canary || stk->right_canary != right_canary)
    {
        nattacks++;
        printf("Attack on Stack(%s) canary!! Canary left = %0x ",
                                stk->stk_name, stk->left_canary);
        printf("right = %0x\n", stk->right_canary);
    }

    if (((CanaryType*)stk->data)[-1] != left_canary ||
       *(CanaryType*) ((stk->data)+stk->Capacity*sizeof(Elemt)) != right_canary)
    {
        nattacks++;
        printf("Attack on Stack(%s) Data massive canary!! Canary left = %0x ",
                                  stk->stk_name,((CanaryType*)stk->data)[-1]);
        printf("right = %0x\n",  *(CanaryType*) ((stk->data)+stk->Capacity*sizeof(Elemt)));
    }

    if (nattacks != 0)
    {
        return ATTACKCANARY;
    }

    return OK;
}

HashType AddHash(Stack* stk)
{
    if (stk == NULL)
    {
       printf("ERROR: AddHash: Stack == NULL\n");
        return NULLSTRSTK;
    }

    size_t Hash = 5381;

    for (size_t i = 0; i < (sizeof(*stk) - sizeof(HashType)); ++i)
    {
        Hash = ((Hash << 5) + Hash) + ((char*)stk)[i];
    }
    stk->StkHash = Hash;

    return Hash;
}

error CheckHash(Stack* stk)
{
    if (stk == NULL)
    {
        printf("ERROR: CheckHash: Stack == NULL\n");
        return NULLSTRSTK;
    }

    HashType PreviousHash = stk->StkHash;

    if (PreviousHash != AddHash(stk))
    {
        printf("ERROR: DDOS ATTACK on %s!!!! MEOW\n", stk->stk_name);
        exit(INCHASH);
    }

    return OK;
}

error ChangeHash(Stack* stk)
{
    if (stk == NULL)
    {
        printf("ERROR: ChangeHash: Stack == NULL\n");
        return NULLSTRSTK;
    }

    stk->StkHash = AddHash(stk);
    return OK;
}
#endif
