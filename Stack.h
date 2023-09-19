#ifndef __STACK_H__
#define __STACK_H__

#define STACK_DUMP(stk) StackDump(stk, __LINE__,__FILE__,__PRETTY_FUNCTION__ )

typedef int Elemt;

enum error {

};
struct Steck {
    size_t Size;
    Elemt* data;
    size_t Capacity;
};

int StackCtor(Steck* stk);
int StackDtor(Steck* stk);
int StackPop(Steck* stk, Elemt* refValue);
int StackPush(Steck* stk, Elemt value);
int StackOK(Steck* stk);
int StackDump(Steck* stk, const size_t nline, const char* namefile, const char* func);


#endif
