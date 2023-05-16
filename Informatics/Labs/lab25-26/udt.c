#include <stdio.h>
#include "udt.h"

void udt_create(Udt *udt){
    udt->head = malloc(sizeof(struct Item));
    udt->size = 0;
    udt->head = udt->tail = NULL;
}


bool udt_is_empty(const Udt *udt){
    return udt->size == 0;
}


void udt_push_front(Udt *udt, int val){
    Item *tmp = (Item*) malloc(sizeof(Item));
    if (tmp == NULL) {
        exit(1);
    }
    tmp->data = val;
    tmp->next = udt->head;
    tmp->prev = NULL;
    if (udt->head) {
        udt->head->prev = tmp;
    }
    udt->head = tmp;
 
    if (udt->tail == NULL) {
        udt->tail = tmp;
    }
    udt->size++;
}


void udt_pop_front(Udt *udt){
    Item *prev;
    if (udt->head == NULL) {
        printf("List is empty");
        return;
    }
 
    prev = udt->head;
    udt->head = udt->head->next;
    if (udt->head) {
        udt->head->prev = NULL;
    }
    if (prev == udt->tail) {
        udt->tail = NULL;
    }
    free(prev);
 
    udt->size--;
}


void udt_push_back(Udt *udt, int val){
    Item *tmp = (Item*) malloc(sizeof(Item));
    if (tmp == NULL) {
        exit(3);
    }
    tmp->data = val;
    tmp->next = NULL;
    tmp->prev = udt->tail;
    if (udt->tail) {
        udt->tail->next = tmp;
    }
    udt->tail = tmp;
 
    if (udt->head == NULL) {
        udt->head = tmp;
    }
    udt->size++;
}


void udt_pop_back(Udt *udt) {
    Item *next;
    if (udt->tail == NULL) {
        printf("List is empty");
        return;
    }
 
    next = udt->tail;
    udt->tail = udt->tail->prev;
    if (udt->tail) {
        udt->tail->next = NULL;
    }
    if (next == udt->head) {
        udt->head = NULL;
    }
    free(next);
 
    udt->size--;
}


void udt_print(const Udt *udt){
    Item *tmp = udt->head;
    while (tmp) {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

size_t udt_size(const Udt *udt){
    return udt->size;
}


Item* getNth(Udt *udt, size_t index) {
    Item *tmp = NULL;
    size_t i;
     
    if (index < udt->size/2) {
        i = 0;
        tmp = udt->head;
        while (tmp && i < index) {
            tmp = tmp->next;
            i++;
        }
    } else {
        i = udt->size - 1;
        tmp = udt->tail;
        while (tmp && i > index) {
            tmp = tmp->prev;
            i--;
        }
    }
 
    return tmp;
}


void udt_insert(Udt *udt, int ind, int val) {
    Item *elm = NULL;
    Item *ins = NULL;
    elm = getNth(udt, ind);
    if (elm == NULL) {
        printf("No such position!");
        return;
    }
    ins = (Item*) malloc(sizeof(Item));
    ins->data = val;
    ins->prev = elm;
    ins->next = elm->next;
    if (elm->next) {
        elm->next->prev = ins;
    }
    elm->next = ins;
 
    if (!elm->prev) {
        udt->head = elm;
    }
    if (!elm->next) {
        udt->tail = elm;
    }
 
    udt->size++;
}


void insertBeforeElement(Udt *udt, Item* elm, int value) {
    Item *ins = NULL;
    if (elm == NULL) {
        printf("List is empty");
        return;
    }
     
    if (!elm->prev) {
        udt_push_front(udt, value);
        return;
    }
    ins = (Item*) malloc(sizeof(Item));
    ins->data = value;
    ins->prev = elm->prev;
    elm->prev->next = ins;
    ins->next = elm;
    elm->prev = ins;
 
    udt->size++;
}


void udt_sort(Udt *udt) {
    if(udt->head == NULL){
        printf("List is empty");
        return;
    }
    Udt out;
    udt_create(&out);
    Item *sorted = NULL;
    Item *unsorted = NULL;
     
    udt_push_front(&out, udt->head->data);
    udt_pop_front(udt);
 
    unsorted = udt->head;
    while (unsorted) {
        sorted = out.head;      
        while (sorted && !(unsorted->data < sorted->data)) {
            sorted = sorted->next;
        }
        if (sorted) {
            insertBeforeElement(&out, sorted, unsorted->data);
        } else {
            udt_push_back(&out, unsorted->data);
        }
        unsorted = unsorted->next;
    }
    
    *udt = out;
}
