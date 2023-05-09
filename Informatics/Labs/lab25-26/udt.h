#ifndef _UDT_H_
#define _UDT_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct Node
{
    int value;
    struct Node *next;
    
} Node;

typedef struct udt
{
    
    Node *head;
    size_t size;

} udt;

void udt_create(udt *);
bool udt_is_empty(const udt *);
void udt_push_front(udt *);
void udt_push_back(udt *);
void udt_pop_front(udt *);
void udt_pop_back(udt *);
void udt_print(const udt *);
size_t udt_size(const udt *);
void udt_insert(udt *, const Node);

#endif