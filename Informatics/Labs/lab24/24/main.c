#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "libs/item.h"
#include "libs/stack.h"
#include "libs/tree.h"
bool IsDigit(char c) {
    return c >= '0' && c <= '9';
}

bool IsOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == ')';
}

bool IsLetter(char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

double DoOperation(double a, double b, char operator) {
    double c;
    if (operator == '+') {
        c = a + b;
    } else if (operator == '-') {
        c = a - b;
    } else if (operator == '*') {
        c = a * b;
    } else if (operator == '/') {
        c = a / b;
    } else if (operator == '^') {
        c = pow(a, b);
    }
    return c;
}

int CheckPriorety(char c) {
    int result;
    if (c == '+' || c == '-') {
        result = 1;
    } else if (c == '*' || c == '/') {
        result = 2;
    } else if (c == '^') {
        result = 3;
    } else if (c == ')') {
        result = -1;
    } else if (c == '(') {
        result = 0;
    }
    return result;
}

void ReadToStack(Stack **stk) {
    Stack *bufStk = NULL;
    bool isNumTyped = false;
    bool isNotStarted = true;
    bool isFractional = false;
    bool nextIsOp = false;
    double buff = 0.0;
    int sign = 1;
    double p = 1.0;
    while (1) {
        char c = getchar();
        if (c == ' ') {
            continue;
        }
        if (IsDigit(c) || c == '.' || (c == '+' || c == '-') && isNotStarted && !nextIsOp) {
            isNotStarted = false;
            if (IsDigit(c)) {
                isNumTyped = true;
                buff = buff * 10 + (c - '0');
                if (isFractional) {
                    p *= 0.1;
                }
            } else if (c == '.') {
                isFractional = true;
            } else if (c == '-') {
                sign = -1;
            }
        } else if (IsOperator(c) || c == EOF || c == '\n') {
            isNumTyped = false;
            Value v;
            if (!nextIsOp) {
                buff = buff * p * sign;
                v.number = buff;
                StackPush(stk, (Item){v, NUMBER});
            }
            isNotStarted = true;
            sign = 1;
            buff = 0.0;
            p = 1.0;
            nextIsOp = false;
            isFractional = false;
            int priorety = CheckPriorety(c);
            while (!StackIsEmpty(&bufStk)) {
                if (CheckPriorety(StackTop(&bufStk).value.symbol) > priorety || c == EOF || c == '\n') {
                    if (StackTop(&bufStk).value.symbol != '(') {
                        StackPush(stk, StackTop(&bufStk));
                    }
                    StackPop(&bufStk);
                } else {
                    break;
                }
            }
            if (c == ')' || c == EOF || c == '\n') {
                nextIsOp = true;
            } else {
                v.symbol = c;
                StackPush(&bufStk, (Item){v, OPERATOR});
            }

        } else if (c == '(') {
            Value v;
            v.symbol = c;
            StackPush(&bufStk, (Item){v, OPERATOR});
        } else if (IsLetter(c)) {
            nextIsOp = true;
            Value v;
            v.symbol = c;
            StackPush(stk, (Item){v, VARIABLE});
            if (!isNotStarted) {
                if (!isNumTyped) {
                    buff = 1.0;
                }
                buff *= p * sign;
                v.number = buff;
                StackPush(stk, (Item){v, NUMBER});
                v.symbol = '*';
                StackPush(stk, (Item){v, OPERATOR});
            }
            isNumTyped = true;
        }

        if (c == '\n' || c == EOF) {
            break;
        }
        
    }
}

void BuildTree(Node **root, Stack **expr) {
    if (StackIsEmpty(expr)) {
        return;
    }

    (*root) = CreateNode(StackTop(expr));
    
    StackPop(expr);
    if ((*root)->value.type == OPERATOR) {
        BuildTree(&(*root)->r, expr);
        BuildTree(&(*root)->l, expr);
    }

}

void PrintTree(Node *root) {
    if (root != NULL) {
        if (root->value.type == OPERATOR) {
            if (root->l->value.type == OPERATOR) {
                if (CheckPriorety(root->value.value.symbol) > CheckPriorety(root->l->value.value.symbol)) {
                    printf("(");
                }
            }

            PrintTree(root->l);

            if (root->l->value.type == OPERATOR) {
                if (CheckPriorety(root->value.value.symbol) > CheckPriorety(root->l->value.value.symbol)) {
                    printf(")");
                }
            }
            printf(" %c ", root->value.value.symbol);

            if (root->r->value.type == OPERATOR) {
                if (CheckPriorety(root->value.value.symbol) > CheckPriorety(root->r->value.value.symbol)) {
                    printf("(");
                }
            }

            PrintTree(root->r);

            if (root->r->value.type == OPERATOR) {
                if (CheckPriorety(root->value.value.symbol) > CheckPriorety(root->r->value.value.symbol)) {
                    printf(")");
                }
            }
        } else if (root->value.type == VARIABLE) {
            printf("%c", root->value.value.symbol);
        } else {
            printf("%f", root->value.value.number);
        }
    } else {
        printf("\n");
    }
}

int main() {
    Stack *stk = NULL;
    Value v;
    v.number = 5;
    Node *root = NULL;
    ReadToStack(&stk);
    Stack *iter = stk;
    BuildTree(&root, &stk);
    PrintTree(root);
}