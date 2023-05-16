#ifndef __STACK__
#define __STACK__
#include <stdbool.h>
#include "item.h"

typedef struct Stack {
    Item value;
    struct Stack *next;
} Stack;

bool StackIsEmpty(Stack **top);
Item StackTop(Stack **top);
void StackPush(Stack **top, Item value);
void StackPop(Stack **top);
void StackPrint(Stack *top);


#endif