//#define DEBUG
//#define NO_POIZON
//#define NO_CANARY
//#define NO_VERIFY
typedef double StackElem_t;
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include "stack.h"

int main()
{
    stack_t stk1 = {};
    StackCtor(&stk1, 5);

    StackPush(&stk1, 10);
    StackPush(&stk1, 20);

    //StackCtor(&stk1, 5);

    StackElem_t last = 0;
    StackPop(&stk1, &last);
    printf("%lg", last);
    StackPush(&stk1, 30);
    for (int i = 0; i < 30; ++i)
    {
        StackPush(&stk1, 0.657);
    }

    for (int i = 0; i < 50; ++i)
    {
        StackPop(&stk1, &last);
    }

    for (int i = 0; i < 30; ++i)
    {
        StackPush(&stk1, 0.657);
    }
    StackCtor(&stk1, 5);
    //PRINT_STACK(&stk1, lg);

    StackDtor(&stk1);
    //StackDtor(&stk1);

    return 0;
}
