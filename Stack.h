#ifndef __STACK_H__
#define __STACK_H__

#define STACK_DUMP(stk) StackDump(stk, __LINE__,__FILE__,__PRETTY_FUNCTION__ )

#define CHECKERROR(exp) do {                             \
                        if (exp)                         \
                         {                               \
                             printf("ERROR:" #exp "\n");      \
                             n_error += 1;                \
                         }                               \
                        } while(0)

#define STACK_CTOR(stk, capacity) StackCtor((stk), (capacity), #stk, \
                                  __LINE__,__FILE__,__PRETTY_FUNCTION__)

typedef int Elemt;

enum error {
    NULLSTRSTK = 1,
    SIZELESSZERO = 2,
    CAPACITYLESSZERO = 3,
    CAPACITYLESSSIZE = 4,
    CAPACITYEQUALZERO = 5,
    INCORRECTCAPACITY = 7,
    NODINMEMORY = 6,
    ERROR = -1,
    OK = 0
};
struct Stack {
    const char* stk_name;
    const char* birth_function;
    size_t birth_line;
    const char* birth_file;
    size_t Size;
    Elemt* data;
    size_t Capacity;
};

error StackCtor(Stack* stk, const size_t Capacity, const char* name,
                const size_t birth_line, const char* birth_file, const char* birth_function);
error StackDtor(Stack* stk);
error StackPop(Stack* stk, Elemt* refValue);
error StackPush(Stack* stk, Elemt value);
error StackOK(const Stack* stk);
error StackDump(Stack* stk, const size_t nline, const char* namefile, const char* func);


#endif
