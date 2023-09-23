#ifndef __STACK_H__
#define __STACK_H__


#ifndef RELEASE
    #define STACK_DUMP(stk) STACK_DUMP(stk)
#else
    STACK_DUMP(stk) ERROR
#endif

#define STACK_DUMP(stk) StackDump((stk), __LINE__,__FILE__,__PRETTY_FUNCTION__ )

#define STACK_CTOR(stk, capacity) StackCtor((stk), (capacity), #stk, \
                                  __LINE__,__FILE__,__PRETTY_FUNCTION__)

#define STACKOK(stk) StackOK((stk), __LINE__,__FILE__,__PRETTY_FUNCTION__)


const unsigned long long right_canary = 0xB0BABABE;
const unsigned long long left_canary  = 0xDEADFEED;

typedef int Elemt;

enum error {
    NULLSTRSTK = 1,
    SIZELESSZERO = 2,
    CAPACITYLESSZERO = 3,
    CAPACITYLESSSIZE = 4,
    CAPACITYEQUALZERO = 5,
    INCORRECTCAPACITY = 7,
    NODINMEMORY = 6,
    ADRESSNULL = 8,
    SIZEEQUALZERO = 9,
    ERROR = -1,
    OK = 0
};

#ifndef RELEASE
struct Stack {
    unsigned long long right_canary = right_canary;
    const char* stk_name;
    const char* birth_function;
    size_t birth_line;              // add isStackCtor and isStackDtor
    const char* birth_file;
    size_t Size;
    Elemt* data;
    size_t Capacity;
    unsigned long long left_canary = left_canary;
};
#else
struct Stack {
    const char* stk_name;
    const char* birth_function;
    size_t birth_line;              // add isStackCtor and isStackDtor
    const char* birth_file;
    size_t Size;
    Elemt* data;
    size_t Capacity;
};
#endif

error StackCtor(Stack* stk, const size_t Capacity, const char* name,
                const size_t birth_line, const char* birth_file, const char* birth_function);
error StackDtor(Stack* stk);
error StackPop(Stack* stk, Elemt* refValue);
error StackPush(Stack* stk, Elemt value);
error StackOK(const Stack* stk, const size_t line,
              const char* namefile, const char* func);
error StackDump(Stack* stk, const size_t nline,
                const char* namefile, const char* func);

#endif
