#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "stack.h"
#include "item.h"

bool StackIsEmpty(Stack **top) {
    return *top == NULL;
}

Item StackTop(Stack **top) {
    return (*top)->value;
}

void StackPush(Stack **top, Item value) {
    Stack *new = (Stack *)malloc(sizeof(Stack));
    new->next = *top;
    new->value = value;
    *top = new;
}

void StackPop(Stack **top) {
    if (!StackIsEmpty(top)) {
        Stack *buf = *top;
        *top = (*top)->next;
        free(buf);
    }
}