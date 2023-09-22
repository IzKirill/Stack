#ifndef __STACK_H__
#define __STACK_H__

#define STACK_DUMP(stk) StackDump(stk, __LINE__,__FILE__,__PRETTY_FUNCTION__ )

#define checkerror(exp) do {                             \
                        if (exp)                         \
                         {                               \
                             printf("ERROR:" #exp "\n");      \
                             n_error += 1;                \
                         }                               \
                        } while(0)
typedef int Elemt;

enum error {
    NULLSTRSTK = 1,
    SIZELESSZERO = 2,
    CAPACITYLESSZERO = 3,
    CAPACITYLESSSIZE = 4,
    CAPACITYEQUALZERO = 5,
    ERROR = -1,
    OK = 0
};
struct Steck {
    size_t Size;
    Elemt* data;
    size_t Capacity;
};

error StackCtor(Steck* stk, const size_t Capacity);
error StackDtor(Steck* stk);
error StackPop(Steck* stk, Elemt* refValue);
error StackPush(Steck* stk, Elemt value);
error StackOK(const Steck* stk);
error StackDump(Steck* stk, const size_t nline, const char* namefile, const char* func);


#endif
