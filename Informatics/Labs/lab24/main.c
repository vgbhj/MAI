#include <stdio.h>
#include <stdlib.h>


#define CMP_EQ(a, b) ((a) == (b))
#define CMP_LT(a, b) ((a) < (b))
#define CMP_GT(a, b) ((a) > (b))


typedef struct Node {
  int data;
  struct Node *left; // min
  struct Node *right; // max
  struct Node *parent;
  
} Node;


Node* getFreeNode(int value, Node *parent) {
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
        if (CMP_GT(value, tmp->data)) {
            if (tmp->right) {
                tmp = tmp->right;
                continue;
            } else {
                tmp->right = getFreeNode(value, tmp);
                return;
            }
        } else if (CMP_LT(value, tmp->data)) {
            if (tmp->left) {
                tmp = tmp->left;
                continue;
            } else {
                tmp->left = getFreeNode(value, tmp);
                return;
            }
        } else {
            exit(2);
        }
    }
}



int main(){

}