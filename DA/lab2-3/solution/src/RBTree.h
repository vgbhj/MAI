#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

class RBTree {
public:
    RBTree();
    ~RBTree();

    void insertValue(std::string key, unsigned long long value);
    void deleteValue(std::string key);
    unsigned long long searchValue(std::string key);
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void clear();

private:
    enum Color { RED, BLACK };

    struct Node {
        std::string key;
        unsigned long long value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(std::string key, unsigned long long value)
            : key(key), value(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    void rotateLeft(Node* &root, Node* &pt);
    void rotateRight(Node* &root, Node* &pt);
    void fixInsertRBTree(Node* &root, Node* &pt);
    Node* BSTInsert(Node* root, Node* pt);
    void inorderHelper(Node* root);
    Node* searchHelper(Node* root, std::string key);
    void deleteNodeHelper(Node* &root, std::string key);
    void clearHelper(Node* &node);

    void saveHelper(std::ofstream &outFile, Node* node);
    void loadHelper(std::ifstream &inFile);

    static std::string toLower(std::string str);
};

#endif // RBTREE_H
