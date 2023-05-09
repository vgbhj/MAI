#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct NodeList {
    void *value;
    struct NodeList *next;
    struct NodeList *prev; 
} NodeList;


typedef struct _DblLinkedList {
    size_t size;
    NodeList *head;
    NodeList *tail;
} DblLinkedList;

DblLinkedList* createDblLinkedList();
void deleteDblLinkedList(DblLinkedList **list);
void pushFront(DblLinkedList *list, void *data);
void* popFront(DblLinkedList *list);
void pushBack(DblLinkedList *list, void *value);
void* popBack(DblLinkedList *list);

#endif