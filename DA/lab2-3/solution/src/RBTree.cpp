#include "RBTree.h"

RBTree::RBTree(){
    NullNode = new Node();
    NullNode->color = BLACK;
    root = NullNode;
}

RBTree::~RBTree(){
    erace(root);
    delete NullNode;
}

void RBTree::rotateLeft(Node * x){
    Node * xRight = x->right;
    x->right = xRight->left;
    if (xRight->left != NullNode) {
        xRight->left->parent = x;
    }
    xRight->parent = x->parent;
    if (x->parent == nullptr) {
        root = xRight;
    }
    else if (x == x->parent->left) {
        x->parent->left = xRight;
    }
    else {
        x->parent->right = xRight;
    }
    xRight->left = x;
    x->parent = xRight;
}

void RBTree::rotateRight(Node * x){
    Node * xLeft = x->left;
    x->left = xLeft->right;
    if (xLeft->right != NullNode) {
        xLeft->right->parent = x;
    }
    xLeft->parent = x->parent;
    if (x->parent == nullptr) {
        root = xLeft;
    }
    else if (x == x->parent->right) {
        x->parent->right = xLeft;
    }
    else {
        x->parent->left = xLeft;
    }
    xLeft->right = x;
    x->parent = xLeft;
}

Node * RBTree::minRightNode(Node * x){
    while(x->left != NullNode){
        x = x->left;
    }
    return x;
}

Node * RBTree::maxLeftNode(Node * x){
    while(x->right != NullNode){
        x = x->right;
    }
    return x;
}

void RBTree::fixInsertRBTree(Node * x){
    while(x->parent != nullptr && x->parent->color == RED){
        if(x->parent == x->parent->parent->left){
            Node * uncle = x->parent->parent->right;
            if(uncle->color == RED){
                uncle->color = BLACK;
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {
                if(x == x->parent->right){
                    x = x->parent;
                    rotateLeft(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(x->parent->parent);
            }
        } else {
            Node * uncle = x->parent->parent->left;
            if(uncle->color == RED){
                uncle->color = BLACK;
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            } else {
                if(x == x->parent->left){
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(x->parent->parent);
            }
        }
    }
    root->color = BLACK;
}


void RBTree::fixDeleteRBTree(Node * x) {
    Node * bro;
    while(x != root && x->color == BLACK){
        if(x == x->parent->left){
            bro = x->parent->right;
            if(bro->color == RED){
                bro->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                bro = x->parent->right; 
            }
            if(bro->left->color == BLACK && bro->right->color == BLACK){
                bro->color = RED;
                x = x->parent;
            } else {
                if(bro->right->color == BLACK){
                    bro->left->color = BLACK;
                    bro->color = RED;
                    rotateRight(bro);
                    bro = x->parent->right;
                }
                bro->color = x->parent->color;
                x->parent->color = BLACK;
                bro->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            bro = x->parent->left;
            if(bro->color == RED){
                bro->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                bro = x->parent->left;
            }
            if(bro->right->color == BLACK && bro->left->color == BLACK){
                bro->color = RED;
                x = x->parent;
            } else {
                if(bro->left->color == BLACK){
                    bro->right->color = BLACK;
                    bro->color = RED;
                    rotateLeft(bro);
                    bro = x->parent->left;
                }
                bro->color = x->parent->color;
                x->parent->color = BLACK;
                bro->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

string RBTree::insertValue(const string& key, ull value){
    Node * newNode = new Node();
    newNode->key = key;
    newNode->value = value;
    newNode->left = NullNode;
    newNode->right = NullNode;
    newNode->parent = nullptr;
    newNode->color = RED;

    Node * curr = root;
    Node * currPar = nullptr;

    while(curr != NullNode){
        currPar = curr;
        int comp = compareString(curr->key, key);
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
        int comp = compareString(newNode->key, currPar->key);
        if(comp == 1){
            currPar->right = newNode;
        } else {
            currPar->left = newNode;
        }
    }

    if(newNode->parent == nullptr){
        newNode->color = BLACK;
        return "OK\n";
    } else if(newNode->parent->parent == nullptr){
        return "OK\n";
    }
    fixInsertRBTree(newNode);
    return "OK\n";
}

string RBTree::deleteValue(const string &key){
    Node * curr = root;
    Node * rem = NullNode, * fix, * temp;
    while(curr != NullNode){
        int comp = compareString(curr->key, key);
        if(comp == 1){
            curr = curr->left;
        } else if (comp == -1){
            curr = curr->right;
        } else {
            rem = curr;
            break;
        }
    }
 
    if(rem == NullNode){
        return "NoSuchWord\n";
    }
        
    temp = rem;
    Tcolor TempColor = temp->color;
    if(rem->left == NullNode){
        fix = rem->right;
        if(rem->parent == nullptr){
            root = rem->right;
        } else {
            if(rem == rem->parent->left){
                rem->parent->left = rem->right;
            } else {
                rem->parent->right = rem->right;
            }
        }
        rem->right->parent = rem->parent;
    } else if(rem->right == NullNode){
        fix = rem->left;
        if(rem->parent == nullptr){
            root = rem->left;
        } else {
            if(rem == rem->parent->left){
                rem->parent->left = rem->left;
            } else {
                rem->parent->right = rem->left;
            }
        }
        rem->left->parent = rem->parent;
    } else {
        temp = minRightNode(rem->right);
        TempColor = temp->color;
        fix = temp->right;
        if(temp->parent == rem){
            fix->parent = temp;
        } else {
            if(temp->parent == nullptr){
                root = temp->right;
            } else {
                if(temp == temp->parent->left){
                    temp->parent->left = temp->right;
                } else {
                    temp->parent->right = temp->right;
                }
            }
            temp->right->parent = temp->parent;
            temp->right = rem->right;
            temp->right->parent = temp;
        }
        if(rem->parent == nullptr){
            root = temp;
        } else {
            if(rem == rem->parent->left){
                rem->parent->left = temp;
            } else {
                rem->parent->right = temp;
            }
        }
        temp->parent = rem->parent;
        temp->left = rem->left;
        temp->left->parent = temp;
        temp->color = rem->color;
    }
    delete rem;
    if(TempColor == BLACK){
        fixDeleteRBTree(fix);
    }
    return "OK\n";
}

void RBTree::erace(Node * x){
    if(x == NullNode){
        return;
    }
    if(x->left != NullNode){
        erace(x->left);
    }
    if(x->right != NullNode){
        erace(x->right);
    }
    delete x;
    root = NullNode;
}

void RBTree::get(const string &key){
    Node * curr = root;
    while(curr != NullNode){
        int comp = compareString(curr->key, key);
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
    if(x == NullNode){
        return;
    } else {
        size_t n = x->key.size();
        string key = x->key;
        ull value = x->value;
        file.write((char *)&n, sizeof(size_t));
        file.write(key.c_str(), sizeof(char) * n);
        file.write((char *)&value, sizeof(ull));
        saveFile(file, x->left);
        saveFile(file, x->right);
    }
}

void RBTree::loadFile(ifstream &file){
    erace(root);
    if(file.peek() == EOF){
        cout << "OK\n";
        return;
    }
    size_t n;
    string key;
    ull value;
    file.read((char *)&n, sizeof(size_t));
    while(n != -1){
        key.clear();
        key.resize(n);
        file.read((char *)key.data(), sizeof(char) * n);
        file.read((char *)&value, sizeof(ull));
        insertValue(key, value);
        file.read((char *)&n, sizeof(size_t));
    }
    cout << "OK\n";
}

void RBTree::print(Node * x, int level){
    if(x != NullNode){
        cout << level << " " << x->key << " " << x->value << " " << x->color << "\n";
        level++;
        print(x->left, level);
        print(x->right, level);
        cout << "--" << x->key << "--\n";
    }
}

std::string RBTree::toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int RBTree::compareString(const string &x, const string &y){
    if(x.length() > y.length()){
        return 1;
    } else if (x.length() < y.length()){
       return -1;
    } else {
    for(int i = 0; i < x.length(); i++){
        if(x[i] > y[i]){
            return 1;
        } else if (x[i] < y[i]){
            return -1;
        }
    }
    }
    return 0;
}