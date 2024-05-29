#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>

class RedBlackTree {
public:
    enum Color { RED, BLACK };
    
    class Node {
    public:
        std::string key;
        unsigned long long value;
        Color color;
        Node *left, *right, *parent;

        Node(const std::string& k, unsigned long long v) 
        : key(k)
        , value(v)
        , color(RED)
        , left(nullptr)
        , right(nullptr)
        , parent(nullptr) 
        {}

        bool isOnLeft() { return this == parent->left; }
    
        // returns pointer to uncle
        Node *uncle() {
            // If no parent or grandparent, then no uncle
            if (parent == NULL or parent->parent == NULL)
            return NULL;
        
            if (parent->isOnLeft())
            // uncle on right
            return parent->parent->right;
            else
            // uncle on left
            return parent->parent->left;
        }
        
        // returns pointer to sibling
        Node *sibling() {
            // sibling null if no parent
            if (parent == NULL)
            return NULL;
        
            if (isOnLeft())
            return parent->right;
        
            return parent->left;
        }
        
        // moves node down and moves given node in its place
        void moveDown(Node *nParent) {
            if (parent != NULL) {
            if (isOnLeft()) {
                parent->left = nParent;
            } else {
                parent->right = nParent;
            }
            }
            nParent->parent = parent;
            parent = nParent;
        }
        
        bool hasRedChild() {
            return (left != NULL and left->color == RED) or
                (right != NULL and right->color == RED);
        }
};
    RedBlackTree();
    ~RedBlackTree();
    void insert(std::string k, unsigned long long v);

    void inorder() { inorderHelper(root); }
    Node* search(std::string key)
    {
        key = toLower(key);
        return searchHelper(root, key);
    }
    void clear();
    static Node *successor(Node *x);
    void deleteByKey(std::string key) {

        key = toLower(key);
        if (root == NULL)
            return;
        Node *v = search(key), *u;
        std::cout << v->key;
        if (v->key != key) {
            // std::cout << "No node found to delete with value:" << n << endl;
            return;
        }
        
        deleteNode(v);
    }

//         // prints inorder of the tree
    // void printInOrder() {
    //     std::cout << "Inorder: " << endl;
    //     if (root == NULL)
    //     cout << "Tree is empty" << endl;
    //     else
    //     inorder(root);
    //     cout << endl;
    // }
    
    // // prints level order of the tree
    // void printLevelOrder() {
    //     cout << "Level order: " << endl;
    //     if (root == NULL)
    //     cout << "Tree is empty" << endl;
    //     else
    //     levelOrder(root);
    //     cout << endl;
    // }
    
private:
    Node* root;
    Node* NIL;
    
    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void fixInsert(Node* k);
    void inorderHelper(Node* node);
    Node* searchHelper(Node* node, std::string key);

    static std::string toLower(std::string str);


    void clearHelper(Node* &node);

    
    void swapColors(Node *x1, Node *x2);
    void swapValues(Node *u, Node *v);
    void fixRedRed(Node *x);
    Node *BSTreplace(Node *x);
    void deleteNode(Node *v);
    void fixDoubleBlack(Node *x);
};
#endif // RBTREE_H