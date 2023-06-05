#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef int T;
 
typedef struct _Node {
    T data;
    int ind;
    struct _Node *next;
    struct _Node *prev;
} Node;
 
typedef struct _Ring {
    size_t size;
    Node *current;
} Ring;
 
Ring* createRing() {
    Ring *tmp = (Ring*)malloc(sizeof(Ring));
    if(tmp == NULL){
        return NULL;
    } 
    tmp->size = 0;
    tmp->current = NULL;
 
    return tmp;
}

Node* next(Ring *ring) {
    Node* retVal = NULL;
    if (ring->current) {
        ring->current = ring->current->next;
        retVal = ring->current;
    } 
    return retVal;
}
 
Node* prev(Ring *ring) {
    Node* retVal = NULL;
    if (ring->current) {
        ring->current = ring->current->prev;
        retVal = ring->current;
    }
    return retVal;
}


void circleNode(Node *node, void (*f)(Node* node)) {
    Node *anchor = node;
    if (anchor) {       
        do {
            f(anchor);
            anchor = anchor->next;
        } while (anchor != node);
    }
}

void findInd(Ring* ring, int ind) {
    Node *anchor = ring->current;
        if (anchor) {
            do {
                if(ring->current->ind == ind){
                    break;
                }
                ring->current = ring->current->next;
            } while (anchor != ring->current);
        }
}
 
void printNode(Node *node) {
    printf("%d ", node->data);


    // Debug
    // printf("%d %d \n", node->data, node->ind);
}

void updateIndAfterAdd(Node *node){

    // Debug
    // printf("id1: %d %d \n", node->data, node->ind);
    // printf("id2: %d %d \n", node->prev->data, node->ind);
    if(node->ind == node->prev->ind){
        node->ind++;
    }
}

void updateIndAfterDel(Node *node){
    // Debug
    // printf("id: %d %d \n", node->data, node->ind);
    if( abs(node->ind - node->prev->ind) != 1 && node->ind != 0){
        node->ind--;
    }
}

void addElement(Ring *ring, T value) {
    Node *prev = NULL;
    Node *tmp = (Node*)malloc(sizeof(Node));
    tmp->ind = 0;
 
    tmp->data = value;
    //Если в кольце нет элементов
    if (ring->current == NULL) {
        ring->current = tmp;
        tmp->next = tmp->prev = tmp;          
    } else {
        tmp->ind = ring->current->ind;
        tmp->next = ring->current;
        tmp->prev = ring->current->prev;
        ring->current->prev->next = tmp;
        ring->current->prev = tmp;
        ring->current = tmp;
        circleNode(ring->current->next, updateIndAfterAdd);
    }
    ring->size++;
}

T removeElement(Ring *ring) {
    Node *afterTarget = NULL;
    T retVal;
 
    if (ring->current == NULL) {
        exit(1);
    }
 
    //Если в кольце последний элемент
    if (ring->current->next == ring->current) {
        retVal = ring->current->data;
        free(ring->current);
        ring->current = NULL;            
    } else {
        afterTarget = ring->current->next;
        ring->current->prev->next = afterTarget;
        afterTarget->prev = ring->current->prev;
        retVal = ring->current->data;
        free(ring->current);
        ring->current = afterTarget;
        circleNode(ring->current, updateIndAfterDel);
    }
    ring->size--;
    return retVal;
}

void swapPrevNext(Ring *ring){
        T tmp_data = ring->current->next->data;
        ring->current->next->data = ring->current->prev->data; 
        ring->current->prev->data = tmp_data;
}


int main(){
    Ring *r;
    r = createRing();

    int ind = 0;
    int choose;
    bool g = true;
    while (g) {
        printf("1. Print list\t 2. Insert in list\t 3. Delete from list\t 4. Size\t 5. Task\t 6. Exit\n");
        scanf("%d", &choose);
        switch (choose) {
            case 1: {
                if (r->size == 0) {
                    break;
                }
                findInd(r, 0);
                circleNode(r->current, printNode);
                printf("\n");
                break;
            }
            case 2: {
                T val;
                printf("Write value\n");
                scanf("%d", &val);
                if (r->size != 0) {
                    printf("Write the index where you want insert element\n");
                    scanf("%d", &ind);
                    if (0 <= ind && ind < r->size) {
                        findInd(r, ind);
                        addElement(r, val);
                    } 
                    else {
                        printf("Element with %d index doesn't exists\n", ind);
                        break;
                    }
                }
                else{
                    addElement(r, val);
                }
                break;
            }
            case 3: {
                if (r->size == 0) {
                    printf("List doesn't exists\n");
                    break;
                }
                printf("Write index of deleted element\n");
                scanf("%d", &ind);
                if (0 <= ind && ind < r->size) {
                    findInd(r, ind);
                    removeElement(r);

                } else {
                    printf("Element with %d index doesn't exists\n", ind);
                }
                break;
            }
            case 4: {
                if (r->size == 0) {
                    printf("List doesn't exists\n");
                    break;
                }
                printf("%d \n", r->size);
                break;
            }
            case 5: {
                if (r->size == 0) {
                    printf("List doesn't exists\n");
                    break;
                }
                if(r->size < 3){
                    printf("Too small list for swap =)\n");
                    break;
                }
                int ind;
                printf("Write k value\n");
                scanf("%d", &ind);
                findInd(r, ind);
                swapPrevNext(r);
                break;
            }
            case 6: {
                g = false;
                break;
            }
            default: {
                printf("Wrong answer\n");
            }
        }
    }
    return 0;
}