#ifndef __TREE__
#define __TREE__
#include "item.h"


typedef struct Node {
    Item value;
    struct Node *l;
    struct Node *r;
} Node;

Node *CreateNode(Item value);

#endif