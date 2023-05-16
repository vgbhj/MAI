#ifndef __TREE__
#define __TREE__
#include <stdio.h>
#include "item.h"


typedef struct Node {
    Item value;
    struct Node *right;
    struct Node *left;
} Node;

Node *CreateNode(Item value);
void printTree(Node *root, int h);

#endif