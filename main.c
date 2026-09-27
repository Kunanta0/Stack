typedef double StackElem_t;
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include "stack.h"

int main()
{
    stack_t stk1 = {};
    StackCtor(&stk1, 5);

    StackPush(NULL, 10);
    StackPush(&stk1, 20);

    //StackCtor(&stk1, 5);

    StackElem_t last = 0;
    StackPop(&stk1, &last);
    StackPush(&stk1, 30);
    for (int i = 0; i < 40; ++i)
    {
        StackPush(&stk1, 67);
        PRINT_STACK(&stk1, lg);

        printf("\n\n\n");
    }

    for (int i = 0; i < 52; ++i)
    {
        StackPop(&stk1, &last);
        PRINT_STACK(&stk1, lg);

        printf("\n\n\n");
    }
    StackCtor(&stk1, 5);

    StackDtor(&stk1);

    return 0;
}
