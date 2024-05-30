#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

#include <bits/stdc++.h>
using namespace std;

typedef bool Tcolor;
const Tcolor RED = true;
const Tcolor BLACK = false;

typedef unsigned long long ull;

struct Node{
    Node * parent;
    Node * left;
    Node * right;
    Tcolor color;
    string key;
    ull value;
};


class RBTree
{
    private:
        Node *NullNode;
    protected:
        void rotateLeft(Node *);
        void rotateRight(Node *);
        Node *minRightNode(Node *);
        Node *maxLeftNode(Node *);
        void fixInsertRBTree(Node *);
        void fixDeleteRBTree(Node *);
        int compareString(const string &, const string &);
    public:
        Node *root;
        RBTree();
        ~RBTree();
        string insertValue(const string&, ull);
        string deleteValue(const string&);
        
        void erace(Node * x);
        void get(const string &key);

        void saveFile(ofstream &file, Node * x);
        void loadFile(ifstream &file);

        void print(Node * x, int level);
        
        static std::string toLower(std::string str);
};


#endif //RED_BLACK_TREE_RBTREE_H