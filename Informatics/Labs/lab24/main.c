#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "item.h"
#include "stack.h"
#include "tree.h"

bool IsDigit(char c) {
    return c >= '0' && c <= '9';
}

bool IsOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool IsLetter(char c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
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
    Stack *tmp_stk = NULL;
    double buff = 0.0;
    bool isStart = true;
    bool lstNum = false;
    int sign = 1;
    while (1) {
        char c = getchar();
        if (c == ' ') {
            continue;
        }
        if(IsDigit(c)){
            buff = buff * 10 + (c - '0');
            lstNum = true;
            isStart=false;
        }
        else if (IsOperator(c)){
            if(isStart){
                if(c == '-'){
                    sign = -1;
                    continue;
                }
            }
            if(lstNum){
                Value v;
                v.number = buff * sign;
                StackPush(stk, (Item){v, NUMBER});
                buff = 0.0;
                sign = 1;
            }
            lstNum = false;
            int priorety = CheckPriorety(c);
            while (!StackIsEmpty(&tmp_stk)) {
                if (CheckPriorety(StackTop(&tmp_stk).value.symbol) > priorety || c == EOF || c == '\n') {
                    if (StackTop(&tmp_stk).value.symbol != '(') {
                        StackPush(stk, StackTop(&tmp_stk));
                    }
                    StackPop(&tmp_stk);
                } else {
                    break;
                }
            }
            Value v;
            v.symbol = c;
            StackPush(&tmp_stk, (Item){v, OPERATOR});
            isStart = true;
        }
        else if (c == '(') {
            Value v;
            v.symbol = c;
            StackPush(&tmp_stk, (Item){v, OPERATOR});
            isStart = true;
        }
        else if(c == ')'){  
            if(lstNum){
                Value v;
                v.number = buff * sign;
                StackPush(stk, (Item){v, NUMBER});
                buff = 0.0;
                sign = 1;
            }
            lstNum = false;
            while (!StackIsEmpty(&tmp_stk))
            {
                if(StackTop(&tmp_stk).value.symbol == '('){
                    StackPop(&tmp_stk);
                    break;
                }
                StackPush(stk, StackTop(&tmp_stk));
                StackPop(&tmp_stk);
            }
        }
        else if(IsLetter(c)){
            Value v;
            v.symbol = c;
            StackPush(stk, (Item){v, VARIABLE});
            if(sign < 0){
                buff = 1.0 * sign;
                v.number = buff;
                StackPush(stk, (Item){v, NUMBER});
                v.symbol = '*';
                StackPush(stk, (Item){v, OPERATOR});
                buff = 0.0;
                sign = 1;
            }
        }

        if (c == '\n' || c == EOF) {
            break;
        }

        //Debug
        // printf("%c \n", c);
        // StackPrint(tmp_stk);
        // printf("============\n");
    }

    if(lstNum){
        Value v;
        v.number = buff * sign;
        StackPush(stk, (Item){v, NUMBER});
    }

    while (!StackIsEmpty(&tmp_stk)){    
        StackPush(stk, StackTop(&tmp_stk));
        StackPop(&tmp_stk);
    }
}

void BuildTree(Node **root, Stack **expr) {
    if (StackIsEmpty(expr)) {
        return;
    }

    (*root) = CreateNode(StackTop(expr));
    
    StackPop(expr);
    if ((*root)->value.type == OPERATOR) {
        BuildTree(&(*root)->right, expr);
        BuildTree(&(*root)->left, expr);
    }

}

void ReverseStack(Stack **stk){
    Stack *tmp = NULL;
    while (!StackIsEmpty(stk))
    {
        StackPush(&tmp, StackTop(stk));
        StackPop(stk);
    }
    *stk = tmp;
}

Node* TreeCopy(Node *root){
    Node *new_root = NULL;
    new_root = CreateNode(root->value);
    if(root->left){
        new_root->left = TreeCopy(root->left);    
    }
    if(root->right){
        new_root->right = TreeCopy(root->right);
    }
    return new_root;
}

int TakeSign(double value){
    if(value < 0){
        return -1;
    }
    return 1;
}



int main(){
    Stack *stk = NULL;
    Node *root = NULL;
    ReadToStack(&stk);
    StackPrint(stk);
    printf("\n");
    BuildTree(&root, &stk);
    ConvertTree(&root);
    printTree(root, 0);
}