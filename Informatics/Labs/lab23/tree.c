#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

int max(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

typedef int T;

typedef struct Node {
  int data;
  struct Node *left; // min
  struct Node *right; // max
  struct Node *parent;
  
} Node;


Node* getFreeNode(T value, Node *parent) {
    Node* tmp = (Node*) malloc(sizeof(Node));
    tmp->left = tmp->right = NULL;
    tmp->data = value;
    tmp->parent = parent;
    return tmp;
}


void insert(Node **head, int value) {
    Node *tmp = NULL;
    Node *ins = NULL;
    if (*head == NULL) {
        *head = getFreeNode(value, NULL);
        return;
    }
     
    tmp = *head;
    while (tmp) {
        if ((value > tmp->data)) {
            if (tmp->right) {
                tmp = tmp->right;
                continue;
            } else {
                tmp->right = getFreeNode(value, tmp);
                return;
            }
        } else if (value < tmp->data) {
            if (tmp->left) {
                tmp = tmp->left;
                continue;
            } else {
                tmp->left = getFreeNode(value, tmp);
                return;
            }
        } else {
            printf("Error, value is already in tree!\n");
            tmp = NULL;
        }
    }
}

bool emptyCheck(Node *root){
    if (!root) {
        printf("Tree is empty!\n");
        return true;
    }
    return false;
}

Node* getMinNode(Node *root) {
    while (root->left) {
        root = root->left;
    }
    return root;
}


Node* getMaxNode(Node *root) {
    while (root->right) {
        root = root->right;
    }
    return root;
}


Node *getNodeByValue(Node *root, T value) {
    while (root) {
        if (root->data > value) {
            root = root->left;
            continue;
        } else if ((root->data < value)) {
            root = root->right;
            continue;
        } else {
            return root;
        }
    }
    return NULL;
}


void removeNodeByPtr(Node *target, Node **root) {
    if(target){
        if (target->left && target->right) {
            Node *localMax = getMaxNode(target->left);
            target->data = localMax->data;
            removeNodeByPtr(localMax, root);
            return;
        }
        else if(!target->parent){
            if(target->left){
                target->data = target->left->data;
                removeNodeByPtr(target->left, root);
            }
            else if(target->right){
                target->data = target->right->data;
                removeNodeByPtr(target->right, root);
            }
            else{
                *root = NULL;
            }
            return;

        }
        else if (target->left){
            if (target == target->parent->left) {
                target->parent->left = target->left;
            } else {
                target->parent->right = target->left;
            }
        } else if (target->right) {
            if (target == target->parent->right) {
                target->parent->right = target->right;
            } else {
                target->parent->left = target->right;
            }
        } else {
            if (target == target->parent->left) {
                target->parent->left = NULL;
            } else {
                target->parent->right = NULL;
            }
        }
        free(target);
    }
    else{
        printf("Node isnt in the tree!\n");
    }
}


void deleteValue(Node **root, T value) {
    Node *target = getNodeByValue(*root, value);
    removeNodeByPtr(target, root);
}

void printTree(Node *root, int h) {
    if (root) {
        printTree(root->right, h+1);
        for(int i = 0; i < 4 * h; ++i){
            printf(" ");
        }
        printf("%d\n", root->data);
        printTree(root->left, h+1);
    }
}

int height(Node* root){
    if(root->parent == NULL){
        return 0;
    }
    return height(root->parent)+1;
}

void breadthFind(Node* root) {
    if (emptyCheck(root)) {
        return;
    }
    DblLinkedList *q = createDblLinkedList();
    pushBack(q, root);
    int mx = 0;
    int sm = 0;
    int tmp_h = 0;
    while (q->size != 0) {
        Node *tmp = (Node*) popFront(q);
        if(height(tmp) == tmp_h){
            sm++;
        }
        else{
            mx = max(sm, mx);
            tmp_h = height(tmp);
            sm = 1;
        }
        if (tmp->left) {
            pushBack(q, tmp->left);
        } 
        if (tmp->right) {
            pushBack(q, tmp->right);
        }
    }
    deleteDblLinkedList(&q);

    mx = max(sm, mx);
    printf("%d\n", mx);
}

void print_menu(){
    printf("1. Add node to tree\t 2. Delete node from tree\t 3. Print tree\t 4. Tree breadth.\t 0. Exit \n");
}

int main(){
    Node *root = NULL;
    int value;
    char c;
    print_menu();
    while ((c = getchar()) != EOF)
    {
        if (c == '\n') continue;
        switch (c){
            case '1':
                scanf("%d", &value);
                insert(&root, value);
                break;
            case '2':
                scanf("%d", &value);
                deleteValue(&root, value);
                break;
            case '3':
                printTree(root, 0);
                break;
            case '4':
                breadthFind(root);
                break;
            case '0':
                return 0;
            default:
                printf("Error!\n");
                return 0;
        }
        print_menu();
    }
    
}