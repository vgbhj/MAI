#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

#include <iostream>
using namespace std;

enum Color {RED, BLACK, DOUBLE_BLACK};

struct Node
{
    std::string key;
    unsigned long long value;
    int color;
    Node *left, *right, *parent;

    Node(const std::string& k, unsigned long long v) 
        : key(k)
        , value(v)
        , color(RED)
        , left(nullptr)
        , right(nullptr)
        , parent(nullptr) 
        {}
};

class RBTree
{
    private:
        Node *root;
    protected:
        void rotateLeft(Node *&);
        void rotateRight(Node *&);
        void fixInsertRBTree(Node *&);
        void fixDeleteRBTree(Node *&);
        void inorderBST(Node *&);
        void preorderBST(Node *&);
        int getColor(Node *&);
        void setColor(Node *&, int);
        Node *minValueNode(Node *&);
        Node *maxValueNode(Node *&);
        Node* insertBST(Node *&, Node *&);
        Node* deleteBST(Node *&, string);
        Node* searchHelper(Node *, string);
        int getBlackHeight(Node *);
        static std::string toLower(std::string str);
        void saveHelper(std::ofstream &outFile, Node* node);
        void loadHelper(std::ifstream &inFile);
        void clearHelper(Node* &node);
    public:
        RBTree();
        void insertValue(string, unsigned long long);
        void deleteValue(string);
        void inorder();
        void preorder();
        unsigned long long search(string);
        bool searchBool(string);
        void saveToFile(const std::string& filename);
        void loadFromFile(const std::string& filename);
        void clear();
        void getRoot();
};


#endif //RED_BLACK_TREE_RBTREE_H