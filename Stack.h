#ifndef __STACK_H__
#define __STACK_H__

#ifndef RELEASE
    #define STACK_DUMP(stk) StackDump((stk), __LINE__,__FILE__,__PRETTY_FUNCTION__ )
#else
    #define STACK_DUMP(stk) ERROR
    #define CheckCanary(stk) OK
    #define AddCanary(stk)
    #define AddHash(stk)
    #define ChechHash(stk)  OK
    #define ChangeHash(stk)
#endif

#define STACK_CTOR(stk, capacity) StackCtor((stk), (capacity), #stk, \
                                  __LINE__,__FILE__,__PRETTY_FUNCTION__)

#define STACKOK(stk) StackOK((stk), __LINE__,__FILE__,__PRETTY_FUNCTION__)


typedef unsigned long long CanaryType;
typedef unsigned long long HashType;

const size_t BytesInStactStk = 56;
const CanaryType right_canary = 0xB0BABABE;
const CanaryType left_canary  = 0xDEADFEED;

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
    CANARYDESTROY = 10,
    STACKNOTCTOR = 11,
    STACKDTOR = 12,
    ATTACKCANARY = 13,
    INCHASH = 14,
    ERROR = -1,
    OK = 0
};

#ifndef RELEASE
struct Stack {
    CanaryType left_canary;
    bool isStackCtor;
    bool isStackDtor;
    const char* stk_name;
    const char* birth_function;
    size_t birth_line;
    const char* birth_file;
    size_t Size;
    Elemt* data;
    size_t Capacity;
    CanaryType right_canary;
    HashType StkHash;
};
#else
struct Stack {
    bool isStackCtor;
    bool isStackDtor;
    const char* stk_name;
    const char* birth_function;
    size_t birth_line;
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

#ifndef RELEASE
error StackDump(Stack* stk, const size_t nline,
                const char* namefile, const char* func);

error CheckHash(Stack* stk);
error CheckCanary(Stack* stk);
HashType AddHash(Stack* stk);
error AddCanary(Stack* stk);
error ChangeHash(Stack* stk);
#endif

#endif
