#ifndef _UDT_H_
#define _UDT_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct Item
{
    int data;
    struct Item* prev;
    struct Item* next;
    
} Item;

typedef struct Udt
{
    size_t size;
    Item* head;
    Item* tail;

} Udt;

void udt_create(Udt *);
bool udt_is_empty(const Udt *);
void udt_push_front(Udt *, int val);
void udt_push_back(Udt *, int val);
void udt_pop_front(Udt *);
void udt_pop_back(Udt *);
void udt_print(const Udt *);
size_t udt_size(const Udt *);
void udt_insert(Udt *, int ind, int val);
void udt_sort(Udt *);

#endif