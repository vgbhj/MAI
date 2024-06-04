#include <bits/stdc++.h>
using namespace std;


enum class Color { RED, BLACK };

struct Node{
    Node * parent;
    Node * left;
    Node * right;
    Color color;
    string key;
    uint64_t value;
};


class RBTree
{
    private:
        Node *nil;
    protected:
        void rotateLeft(Node *);
        void rotateRight(Node *);
        Node *minRightNode(Node *);
        Node* insertBST(Node *&, Node *&);
        Node* deleteBST(Node *&,const string&);
        void fixInsertRBTree(Node *);
        void fixDeleteRBTree(Node *);
        int cmpStrings(const string &, const string &);
    public:
        Node *root;
        RBTree();
        ~RBTree();
        string insertValue(const string&, uint64_t);
        string deleteValue(const string&);
        
        void clear(Node *);
        void find(const string &);

        void saveFile(ofstream &file, Node *);
        void loadFile(ifstream &file);
        
        static std::string toLower(std::string str);
};


RBTree::RBTree(){
    nil = new Node{nullptr, nullptr, nullptr, Color::BLACK, "", 0};
    root = nil;
}

RBTree::~RBTree(){
    clear(root);
    delete nil;
}

void RBTree::rotateLeft(Node * node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nil) {
        rightChild->left->parent = node;
    }
    rightChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }
    rightChild->left = node;
    node->parent = rightChild;
}

void RBTree::rotateRight(Node * node){
    Node * leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != nil) {
        leftChild->right->parent = node;
    }
    leftChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = leftChild;
    }
    else if (node == node->parent->right) {
        node->parent->right = leftChild;
    }
    else {
        node->parent->left = leftChild;
    }
    leftChild->right = node;
    node->parent = leftChild;
}

Node * RBTree::minRightNode(Node * node){
    while(node->left != nil){
        node = node->left;
    }
    return node;
}

void RBTree::fixInsertRBTree(Node * node){
    while(node->parent != nullptr && node->parent->color == Color::RED){
        if(node->parent == node->parent->parent->left){
            Node * uncle = node->parent->parent->right;
            if(uncle->color == Color::RED){
                uncle->color = Color::BLACK;
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            } else {
                if(node == node->parent->right){
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateRight(node->parent->parent);
            }
        } else {
            Node * uncle = node->parent->parent->left;
            if(uncle->color == Color::RED){
                uncle->color = Color::BLACK;
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            } else {
                if(node == node->parent->left){
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateLeft(node->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}


void RBTree::fixDeleteRBTree(Node * node) {
    while(node != root && node->color == Color::BLACK){
        if(node == node->parent->left){
            Node * nodeBrother = node->parent->right;
            if(nodeBrother->color == Color::RED){
                nodeBrother->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateLeft(node->parent);
                nodeBrother = node->parent->right; 
            }
            if(nodeBrother->left->color == Color::BLACK && nodeBrother->right->color == Color::BLACK){
                nodeBrother->color = Color::RED;
                node = node->parent;
            } else {
                if(nodeBrother->right->color == Color::BLACK){
                    nodeBrother->left->color = Color::BLACK;
                    nodeBrother->color = Color::RED;
                    rotateRight(nodeBrother);
                    nodeBrother = node->parent->right;
                }
                nodeBrother->color = node->parent->color;
                node->parent->color = Color::BLACK;
                nodeBrother->right->color = Color::BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            Node * nodeBrother = node->parent->left;
            if(nodeBrother->color == Color::RED){
                nodeBrother->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateRight(node->parent);
                nodeBrother = node->parent->left;
            }
            if(nodeBrother->right->color == Color::BLACK && nodeBrother->left->color == Color::BLACK){
                nodeBrother->color = Color::RED;
                node = node->parent;
            } else {
                if(nodeBrother->left->color == Color::BLACK){
                    nodeBrother->right->color = Color::BLACK;
                    nodeBrother->color = Color::RED;
                    rotateLeft(nodeBrother);
                    nodeBrother = node->parent->left;
                }
                nodeBrother->color = node->parent->color;
                node->parent->color = Color::BLACK;
                nodeBrother->left->color = Color::BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = Color::BLACK;
}

string RBTree::insertValue(const string& key, uint64_t value){
    Node * newNode = new Node();
    newNode->key = key;
    newNode->value = value;
    newNode->left = nil;
    newNode->right = nil;
    newNode->parent = nullptr;
    newNode->color = Color::RED;

    Node * curr = root;
    Node * currPar = nullptr;

    while(curr != nil){
        currPar = curr;
        int comp = cmpStrings(curr->key, key);
        if(comp == 1){
            curr = curr->left;
        } else if (comp == -1){
            curr = curr->right;
        } else {
            delete newNode;
            return "Exist\n";
        }
    }

    newNode->parent = currPar;
    if(currPar == nullptr){
        root = newNode;
    } else {
        int comp = cmpStrings(newNode->key, currPar->key);
        if(comp == 1){
            currPar->right = newNode;
        } else {
            currPar->left = newNode;
        }
    }

    if(newNode->parent == nullptr){
        newNode->color = Color::BLACK;
        return "OK\n";
    } else if(newNode->parent->parent == nullptr){
        return "OK\n";
    }
    fixInsertRBTree(newNode);
    return "OK\n";
}

Node* RBTree::deleteBST(Node *&root, const string& key) {
    if (root == nullptr)
        return root;

    int cmp = cmpStrings(key, root->key);
    if (cmp)
        return deleteBST(root->left, key);

    if (cmp)
        return deleteBST(root->right, key);

    if (root->left == nullptr || root->right == nullptr)
        return root;

    Node *temp = minRightNode(root->right);
    root->key = temp->key;
    return deleteBST(root->right, temp->key);
}


string RBTree::deleteValue(const string &key){
    Node *node = deleteBST(root, key);
    fixDeleteRBTree(node);
    return "OK\n";
}

void RBTree::clear(Node * x){
    if(x == nil){
        return;
    }
    if(x->left != nil){
        clear(x->left);
    }
    if(x->right != nil){
        clear(x->right);
    }
    delete x;
    root = nil;
}

void RBTree::find(const string &key){
    Node * curr = root;
    while(curr != nil){
        int comp = cmpStrings(curr->key, key);
        if(comp == 1){
            curr = curr->left;
        } else if (comp == -1){
            curr = curr->right;
        } else {
            cout << "OK: " << curr->value << "\n";
            return;
        }
    }
    cout << "NoSuchWord\n";
}

void RBTree::saveFile(ofstream &file, Node * x){ 
    if(x == nil){
        return;
    } else {
        size_t n = x->key.size();
        string key = x->key;
        uint64_t value = x->value;
        file.write((char *)&n, sizeof(size_t));
        file.write(key.c_str(), sizeof(char) * n);
        file.write((char *)&value, sizeof(uint64_t));
        saveFile(file, x->left);
        saveFile(file, x->right);
    }
}

void RBTree::loadFile(ifstream &file){
    clear(root);
    if(file.peek() == EOF){
        cout << "OK\n";
        return;
    }
    size_t n;
    string key;
    uint64_t value;
    file.read((char *)&n, sizeof(size_t));
    while(n != -1){
        key.clear();
        key.resize(n);
        file.read((char *)key.data(), sizeof(char) * n);
        file.read((char *)&value, sizeof(uint64_t));
        insertValue(key, value);
        file.read((char *)&n, sizeof(size_t));
    }
    cout << "OK\n";
}

std::string RBTree::toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int RBTree::cmpStrings(const string &s1, const string &s2){
    if(s1.length() > s2.length()){
        return 1;
    } else if (s1.length() < s2.length()){
       return -1;
    } else {
    for(int i = 0; i < s1.length(); i++){
        if(s1[i] > s2[i]){
            return 1;
        } else if (s1[i] < s2[i]){
            return -1;
        }
    }
    }
    return 0;
}

int main() {
    // ios::sync_with_stdio(false);
    // cin.tie(0);
    RBTree tree;
    string command, key, path, answer;
    uint64_t value;
    while (cin >> command) {
        try {
            if (command == "+") {
                cin >> key >> value;
                key = tree.toLower(key);
                answer = tree.insertValue(key, value);
                cout << answer;
            } else if (command == "-") {
                cin >> key;
                key = tree.toLower(key);
                answer = tree.deleteValue(key);
                cout << answer;
            } else if (command == "!") {
                cin >> command;
                if (command == "Save") {
                    cin >> path;
                    ofstream file;
                    file.open(path, ios_base::binary);
                    if (!file) throw runtime_error("Unable to open file for writing");
                    tree.saveFile(file, tree.root);
                    cout << "OK\n";
                    size_t i = -1;
                    file.write((char *)&i, sizeof(size_t));
                    file.close();
                } else if (command == "Load") {
                    cin >> path;
                    ifstream file;
                    file.open(path, ios_base::binary);
                    if (!file) throw runtime_error("Unable to open file for reading");
                    tree.loadFile(file);
                    file.close();
                }
            } else {
                key = tree.toLower(command);
                tree.find(key);
            }
        } catch (const bad_alloc &) {
            cout << "ERROR: Not enough memory\n";
        } catch (const runtime_error &e) {
            cout << "ERROR: " << e.what() << '\n';
        } catch (...) {
            cout << "ERROR: n";
        }
    }
    return 0;
}
