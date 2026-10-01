#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include <math.h>

const double CanaryLeft = -675.249;
const double CanaryRight = 52.267;
typedef struct
{
    const char* file;
    int line;
    const char* func;
} log_ctx_t;

#include "log.h"
#include <time.h>
#include <string.h>

#define StackPush(stk, val) Stack_Push((stk), (val), &(log_ctx_t){__FILE__, __LINE__, __func__})
#define StackPop(stk, last) Stack_Pop((stk), (last), &(log_ctx_t){__FILE__, __LINE__, __func__})
#define StackCtor(stk, capacity) Stack_Ctor((stk), (capacity), &(log_ctx_t){__FILE__, __LINE__, __func__})
#define StackDtor(stk) Stack_Dtor((stk), &(log_ctx_t){__FILE__, __LINE__, __func__})

#define PRINT_STACK(stk, specificator)\
{\
    printf("stack with address %p: ", stk);\
    printf("size = %lld, capacity = %lld\ndata with address %p\n{\n", (stk)->size, ((stk)->capacity) - 2, (stk)->data);\
    for (size_t i = 0; i < (stk)->size; ++i)\
    {\
        printf("\tdata[%lld] = %"#specificator"\n", i, (stk)->data[i]);\
    }\
    for (size_t i = (stk)->size; i < (stk)->capacity - 2; ++i)\
    {\
        printf("*\tdata[%lld] = %"#specificator" (POIZON)\n", i, (stk)->data[i]);\
    }\
    printf("}\n");\
}

#define PRINT_STACK_DEBUG(stk, specificator)\
{\
    FILE* log = fopen("log.txt", "a");\
    fprintf(log, "size = %lld, capacity = %lld\ndata with address %p\n{\n", (stk)->size, ((stk)->capacity) - 2, (stk)->data);\
    fprintf(log, "%p\n", (stk));\
    for (size_t i = 0; i < (stk)->size; ++i)\
    {\
        fprintf(log, "\tdata[%lld] = %"#specificator"\n", i, (stk)->data[i]);\
    }\
    for (size_t i = (stk)->size; i < (stk)->capacity - 2; ++i)\
    {\
        fprintf(log, "*\tdata[%lld] = %"#specificator" (POIZON)\n", i, (stk)->data[i]);\
    }\
    fprintf(log, "}\n");\
    fclose(log);\
}

enum ERRORS
{
    OK = 0,
    CALL_CONSTRUCTOR_FOR_EXISTING_STACK =  1,
    STACK_OVERFLOW = 2,
    STACK_IS_NULL = 3,
    DATA_IS_NULL = 4,
    STACK_UNDERFLOW = 5,
    CANARY_DAMAGED = 6
};

typedef struct
{
    StackElem_t* data;
    size_t size;
    size_t capacity;
} stack_t;

int StackVerify(stack_t*, const log_ctx_t* ctx);
int CheckNull(stack_t*, const log_ctx_t* ctx);
int Stack_Ctor(stack_t*, size_t, log_ctx_t* ctx);
void Stack_Push(stack_t*, StackElem_t, log_ctx_t* ctx);
int Stack_Pop(stack_t*, StackElem_t*, log_ctx_t* ctx);
void Stack_Dtor(stack_t*, log_ctx_t* ctx);

int StackVerify(stack_t* stk, const log_ctx_t* ctx)
{
    FILE* log = fopen("log.txt", "a");

    int state = OK;
    if (stk == NULL) state = STACK_IS_NULL;
    if (stk->data == NULL && stk->capacity != 0) state = DATA_IS_NULL;
    if (stk->size > stk->capacity - 2) state = STACK_OVERFLOW;
    //if (stk->data[-1] != CanaryLeft || stk->data[stk->capacity - 2] != CanaryRight) return CANARY_DAMAGED;
    if (state != OK)
    {
        write_log(log, LOG_ERROR, "StackVerify returns ERROR CODE: %d\n", state);
        printf("ERROR: %d in file: %s, line: %d, function^ %s\n", state, ctx->file, ctx->line, ctx->func);
        PRINT_STACK_DEBUG(stk, lg);
        #ifdef DEBUG
        PRINT_STACK(stk, lg);
        #endif
        if (fclose(log) != 0) printf("Error of closing log file\n");
        return state;
    }
    #ifdef DEBUG
    write_log(log, LOG_INFO, "StackVerify returns: %d\n", OK);
    PRINT_STACK_DEBUG(stk, lg);
    PRINT_STACK(stk, lg);
    #endif
    if (fclose(log) != 0) printf("Error of closing log file\n");
    return OK;
}

int CheckNull(stack_t* stk, const log_ctx_t* ctx)
{
    FILE* log = fopen("log.txt", "a");
    int state = OK;

    if (stk == NULL) state = STACK_IS_NULL;

    if (stk->capacity == 0 && stk->size == 0 && stk->data == NULL)
    {
        if (fclose(log) != 0) printf("Error of closing log file\n");
        return OK;
    }
    else state = CALL_CONSTRUCTOR_FOR_EXISTING_STACK;

    write_log(log, LOG_ERROR, "CheckNull returns ERROR CODE: %d\n", state);
    printf("ERROR: %d in file: %s, line: %d, function^ %s\n", state, ctx->file, ctx->line, ctx->func);
    PRINT_STACK_DEBUG(stk, lg);
    #ifdef DEBUG
    PRINT_STACK(stk, lg);
    #endif

    if (fclose(log) != 0) printf("Error of closing log file\n");
    return state;
}

int Stack_Ctor(stack_t* stk, size_t capacity, log_ctx_t* ctx)
{
    int state = CheckNull(stk, ctx);
    assert(state == OK);

    stk->capacity = capacity + 2;
    StackElem_t* ptr = (StackElem_t*)calloc(stk->capacity, sizeof(StackElem_t));
    if (ptr == NULL)
    {
        printf("Error of creating stack with code: %d", DATA_IS_NULL);
        return DATA_IS_NULL;
    }

    stk->data = ptr + 1;
    stk->data[-1] = CanaryLeft;

    for (size_t i = 0; i < capacity; ++i)
    {
        stk->data[i] = NAN;
    }
    stk->data[capacity] = CanaryRight;

    state = StackVerify(stk, ctx);
    assert(state == OK);

    return OK;
}

void Stack_Push(stack_t* stk, StackElem_t val, log_ctx_t* ctx)
{
    int state = StackVerify(stk, ctx);
    assert(state == OK);
    if (stk->size == stk->capacity - 2)
    {
        StackElem_t* ptr = (StackElem_t* )realloc(stk->data - 1, sizeof(StackElem_t) * 2 * (stk->capacity - 1));
        if (ptr == NULL)
        {
            printf("Error of increasing data; element haven't pushed");
            return;
        }
        stk->data = ptr + 1;
        stk->capacity = 2 * (stk->capacity - 1);
        for (size_t i = stk->size; i < stk->capacity; ++i) stk->data[i] = NAN;
        stk->data[stk->capacity - 2] = CanaryRight;
    }

    stk->data[stk->size++] = val;
    state = StackVerify(stk, ctx);
    assert(state == OK);
}

int Stack_Pop(stack_t* stk, StackElem_t* last, log_ctx_t* ctx)
{
    FILE* log = fopen("log.txt", "a");

    int state = StackVerify(stk, ctx);
    assert(state == OK);
    if (stk->size == 0)
    {
        write_log(log, LOG_WARNING, "StackVerify returns warning with code: %d\n", STACK_UNDERFLOW);
        printf("ERROR: %d in file: %s, line: %d, function^ %s\n", STACK_UNDERFLOW, ctx->file, ctx->line, ctx->func);
        PRINT_STACK_DEBUG(stk, lg);
        #ifdef DEBUG
        PRINT_STACK(stk, lg);
        #endif
        if (fclose(log) != 0) printf("Error of closing log file\n");
        return STACK_UNDERFLOW;
    }
    *last = stk->data[--stk->size];
    if (2 * (stk->size + 1) == stk->capacity - 2)
    {
        stk->data = (StackElem_t* )realloc(stk->data, sizeof(StackElem_t) * (stk->size + 1));
        stk->capacity /= 2;
        stk->data[stk->size] = CanaryRight;
    }

    for (size_t i = stk->size; i < stk->capacity; ++i) stk->data[i] = NAN;

    state = StackVerify(stk, ctx);
    assert(state == OK);
    return OK;
}

void Stack_Dtor(stack_t* stk, log_ctx_t* ctx)
{
    int state = StackVerify(stk, ctx);
    assert(state == OK);

    free(--stk->data);
    --stk->data;
    stk->data = NULL;
    stk->capacity = 0;
    stk->size = 0;

    state = CheckNull(stk, ctx);
    assert(state == OK);
}

#endif // STACK_H_INCLUDED
///Канарейки перенеси при реаллокации
