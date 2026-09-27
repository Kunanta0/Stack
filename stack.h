#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

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
    printf("size = %lld\ncapacity = %lld\n", (stk)->size, (stk)->capacity);\
    for (size_t i = 0; i < (stk)->size; ++i)\
    {\
        printf(#stk"[%lld] = %"#specificator"\n", i, (stk)->data[i]);\
    }\
}

enum ERRORS
{
    OK = 0,
    CALL_CONSTRUCTOR_FOR_EXISTING_STACK =  1,
    STACK_OVERFLOW = 2,
    STACK_IS_NULL = 3,
    DATA_IS_NULL = 4,
    STACK_UNDERFLOW = 5
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

    if (stk == NULL) {
            write_log(log, LOG_ERROR, "StackVerify returns ERROR CODE: %d\n", STACK_IS_NULL);
            #ifdef DEBUG
            printf("ERROR: %d in file: %s, line: %d, function^ %s\n", STACK_IS_NULL, ctx->file, ctx->line, ctx->func);
            PRINT_STACK(stk, lg);
            #endif
            if (fclose(log) != 0) printf("Error of closing log file\n");
            return STACK_IS_NULL;
    }
    if (stk->data == NULL && stk->capacity != 0) {
            write_log(log, LOG_ERROR, "StackVerify returns ERROR CODE: %d\n", DATA_IS_NULL);
            #ifdef DEBUG
            printf("ERROR: %d in file: %s, line: %d, function^ %s\n", DATA_IS_NULL, ctx->file, ctx->line, ctx->func);
            PRINT_STACK(stk, lg);
            #endif
            if (fclose(log) != 0) printf("Error of closing log file\n");
            return DATA_IS_NULL;
    }
    if (stk->size > stk->capacity){
            write_log(log, LOG_ERROR, "StackVerify returns ERROR CODE: %d\n", STACK_OVERFLOW);
            #ifdef DEBUG
            printf("ERROR: %d in file: %s, line: %d, function^ %s\n", STACK_OVERFLOW, ctx->file, ctx->line, ctx->func);
            PRINT_STACK(stk, lg);
            #endif
            if (fclose(log) != 0) printf("Error of closing log file\n");
            return STACK_OVERFLOW;
    }
    #ifdef DEBUG
    write_log(log, LOG_INFO, "StackVerify returns: %d\n", OK);
    #endif
    if (fclose(log) != 0) printf("Error of closing log file\n");
    return OK;
}

int CheckNull(stack_t* stk, const log_ctx_t* ctx)
{
    FILE* log = fopen("log.txt", "a");
    if (stk == NULL) {
        write_log(log, LOG_ERROR, "StackVerify returns ERROR CODE: %d\n", STACK_IS_NULL);
        #ifdef DEBUG
        printf("ERROR: %d in file: %s, line: %d, function^ %s\n", STACK_IS_NULL, ctx->file, ctx->line, ctx->func);
        PRINT_STACK(stk, lg);
        #endif
        if (fclose(log) != 0) printf("Error of closing log file\n");
        return STACK_IS_NULL;
    }
    if (stk->capacity == 0 && stk->size == 0 && stk->data == NULL)
    {
        #ifdef DEBUG
        write_log(log, LOG_INFO, "StackVerify returns: %d\n", OK)
        #endif
        if (fclose(log) != 0) printf("Error of closing log file\n");
        return OK;
    }
    else {
        write_log(log, LOG_WARNING, "StackVerify returns warning with code: %d\n", CALL_CONSTRUCTOR_FOR_EXISTING_STACK);
        #ifdef DEBUG
        printf("ERROR: %d in file: %s, line: %d, function^ %s\n", CALL_CONSTRUCTOR_FOR_EXISTING_STACK, ctx->file, ctx->line, ctx->func);
        PRINT_STACK(stk, lg);
        #endif
        if (fclose(log) != 0) printf("Error of closing log file\n");
        return CALL_CONSTRUCTOR_FOR_EXISTING_STACK;
    }
}

int Stack_Ctor(stack_t* stk, size_t capacity, log_ctx_t* ctx)
{
    int state = CheckNull(stk, ctx);
    assert(state == OK);

    stk->capacity = capacity;
    stk->data = (StackElem_t*)calloc(capacity, sizeof(StackElem_t));

    state = StackVerify(stk, ctx);
    assert(state == OK);

    return 0;
}

void Stack_Push(stack_t* stk, StackElem_t val, log_ctx_t* ctx)
{
    int state = StackVerify(stk, ctx);
    assert(state == OK);
    if (stk->size == stk->capacity)
    {
        stk->data = (StackElem_t* )realloc(stk->data, 2 * sizeof(StackElem_t) * stk->capacity);
        stk->capacity *= 2;
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
        #ifdef DEBUG
        printf("ERROR: %d in file: %s, line: %d, function^ %s\n", STACK_UNDERFLOW, ctx->file, ctx->line, ctx->func);
        PRINT_STACK(stk, lg);
        #endif
        if (fclose(log) != 0) printf("Error of closing log file\n");
        return STACK_UNDERFLOW;
    }
    if (2 * stk->size == stk->capacity)
    {
        stk->data = (StackElem_t* )realloc(stk->data, sizeof(StackElem_t) * stk->size);
        stk->capacity /= 2;
    }

    *last = stk->data[stk->size--];

    state = StackVerify(stk, ctx);
    assert(state == OK);
    return OK;
}

void Stack_Dtor(stack_t* stk, log_ctx_t* ctx)
{
    int state = StackVerify(stk, ctx);
    assert(state == OK);

    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    stk->data = 0;

    state = CheckNull(stk, ctx);
    assert(state == OK);
}

#endif // STACK_H_INCLUDED
