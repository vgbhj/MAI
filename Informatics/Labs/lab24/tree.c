#include <stdlib.h>
#include "tree.h"
#include "item.h"

Node *CreateNode(Item value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->value = value;
    return node;
}

void printTree(Node *root, int h) {
    if (root) {
        printTree(root->right, h+1);
        for(int i = 0; i < 4 * h; ++i){
            printf(" ");
        }
        if(root->value.type == NUMBER){
            printf("%.1f\n", root->value.value.number);
        }
        else{
            printf("%c\n", root->value.value.symbol);
        }
        printTree(root->left, h+1);
    }
}