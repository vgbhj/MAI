#include <bits/stdc++.h>
enum Color { RED, BLACK };

class Node {
public:
    std::string key;
    uint64_t value;
    Color color;
    Node *left, *right, *parent;

    Node(std::string key, uint64_t value)
        : key(key), value(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    Node *root;
    Node *TNULL;

    void initializeNULLNode(Node* node, Node* parent) {
        node->key = "";
        node->value = 0;
        node->color = BLACK;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = parent;
    }

    void preOrderHelper(Node* node) {
        if (node != TNULL) {
            std::cout << node->key << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void inOrderHelper(Node* node) {
        if (node != TNULL) {
            inOrderHelper(node->left);
            std::cout << node->key << " ";
            inOrderHelper(node->right);
        }
    }

    void postOrderHelper(Node* node) {
        if (node != TNULL) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            std::cout << node->key << " ";
        }
    }

    Node* searchTreeHelper(Node* node, std::string key) {
        if (node == TNULL || key == node->key) {
            return node;
        }

        if (key < node->key) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    void fixDelete(Node* x) {
        Node* s;
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->right->color == BLACK) {
                        s->left->color = BLACK;
                        s->color = RED;
                        rightRotate(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == RED) {
                    s->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->left->color == BLACK && s->right->color == BLACK) {
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->left->color == BLACK) {
                        s->right->color = BLACK;
                        s->color = RED;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void rbTransplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNodeHelper(Node* node, std::string key) {
        Node* z = TNULL;
        Node* x, * y;
        while (node != TNULL) {
            if (node->key == key) {
                z = node;
            }

            if (node->key <= key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (z == TNULL) {
            std::cout << "NoSuchWord\n";
            return;
        }

        y = z;
        Color y_original_color = y->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == BLACK) {
            fixDelete(x);
        }
        std::cout << "OK\n";
    }

    void fixInsert(Node* k) {
        Node* u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;

                if (u->color == RED) {
                    u->color = BLACK;
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    void printHelper(Node* root, std::string indent, bool last) {
        if (root != TNULL) {
            std::cout << indent;
            if (last) {
                std::cout << "R----";
                indent += "   ";
            } else {
                std::cout << "L----";
                indent += "|  ";
            }

            std::string sColor = root->color ? "BLACK" : "RED";
            std::cout << root->key << "(" << sColor << ")" << std::endl;
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

public:
    RBTree() {
        TNULL = new Node("", 0);
        TNULL->color = BLACK;
        root = TNULL;
    }

    void setRoot(Node* node) {
        root = node;
        if (root != TNULL) {
            root->parent = nullptr;
        }
    }

    void preorder() {
        preOrderHelper(this->root);
    }

    void inorder() {
        inOrderHelper(this->root);
    }

    void postorder() {
        postOrderHelper(this->root);
    }

    Node* searchTree(std::string k) {
        return searchTreeHelper(this->root, k);
    }

    Node* minimum(Node* node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

    Node* maximum(Node* node) {
        while (node->right != TNULL) {
            node = node->right;
        }
        return node;
    }

    Node* successor(Node* x) {
        if (x->right != TNULL) {
            return minimum(x->right);
        }

        Node* y = x->parent;
        while (y != TNULL && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    Node* predecessor(Node* x) {
        if (x->left != TNULL) {
            return maximum(x->left);
        }

        Node* y = x->parent;
        while (y != TNULL && x == y->left) {
            x = y;
            y = y->parent;
        }

        return y;
    }

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void insert(std::string key, uint64_t value) {
        Node* node = new Node(key, value);
        node->parent = nullptr;
        node->key = key;
        node->value = value;
        node->left = TNULL;
        node->right = TNULL;
        node->color = RED;

        Node* y = nullptr;
        Node* x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->key < x->key) {
                x = x->left;
            } else if (node->key > x->key) {
                x = x->right;
            } else {
                std::cout << "Exist\n";
                delete node;
                return;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->key < y->key) {
            y->left = node;
        } else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = BLACK;
            std::cout << "OK\n";
            return;
        }

        if (node->parent->parent == nullptr) {
            std::cout << "OK\n";
            return;
        }

        fixInsert(node);
        std::cout << "OK\n";
    }

    void deleteNode(std::string key) {
        deleteNodeHelper(this->root, key);
    }

    void printTree() {
        if (root) {
            printHelper(this->root, "", true);
        }
    }

    void saveToFile(const std::string& filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "ERROR: Could not open file for writing" << std::endl;
            return;
        }
        saveHelper(outFile, root);
        outFile.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            std::cerr << "ERROR: Could not open file for reading" << std::endl;
            return;
        }
        clear();
        root = loadHelper(inFile, nullptr);
        inFile.close();
    }

    void saveHelper(std::ofstream &outFile, Node* node) {
        if (node == TNULL) {
            outFile.put(0);
            return;
        }
        outFile.put(1);
        size_t keySize = node->key.size();
        outFile.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        outFile.write(node->key.data(), keySize);
        outFile.write(reinterpret_cast<char*>(&node->value), sizeof(node->value));
        outFile.write(reinterpret_cast<char*>(&node->color), sizeof(node->color));
        saveHelper(outFile, node->left);
        saveHelper(outFile, node->right);
    }

    Node* loadHelper(std::ifstream &inFile, Node* parent) {
        char flag;
        inFile.get(flag);
        if (!inFile || flag == 0) {
            return TNULL;
        }
        size_t keySize;
        inFile.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        std::string key(keySize, '\0');
        inFile.read(&key[0], keySize);
        uint64_t value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));
        Color color;
        inFile.read(reinterpret_cast<char*>(&color), sizeof(color));

        Node* node = new Node(key, value);
        node->color = color;
        node->parent = parent;
        node->left = loadHelper(inFile, node);
        node->right = loadHelper(inFile, node);

        return node;
    }

    void clear() {
        clearHelper(root);
        root = TNULL;
    }

    void clearHelper(Node* node) {
        if (node != TNULL) {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }
};

class Dictionary {
private:
    RBTree tree;

    std::string toLowerCase(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return lowerStr;
    }

public:
    void add(const std::string& word, uint64_t number) {
        std::string lowerWord = toLowerCase(word);
        tree.insert(lowerWord, number);
    }

    void remove(const std::string& word) {
        std::string lowerWord = toLowerCase(word);
        tree.deleteNode(lowerWord);
    }

    void find(const std::string& word) {
        std::string lowerWord = toLowerCase(word);
        Node* result = tree.searchTree(lowerWord);
        if (result != nullptr && result->key != "") {
            std::cout << "OK: " << result->value << "\n";
        } else {
            std::cout << "NoSuchWord\n";
        }
    }

    void save(const std::string& filePath) {
        try {
            tree.saveToFile(filePath);
            std::cout << "OK\n";
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << "\n";
        }
    }

    void load(const std::string& filePath) {
        try {
            tree.loadFromFile(filePath);
            std::cout << "OK\n";
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << "\n";
        }
    }
};


int main() {
    Dictionary dict;
    std::string command;

    while (std::getline(std::cin, command)) {
        std::istringstream iss(command);
        std::string operation, word, filePath;
        uint64_t number;

        iss >> operation;
        if (operation == "+") {
            iss >> word >> number;
            dict.add(word, number);
        } else if (operation == "-") {
            iss >> word;
            dict.remove(word);
        } else if (operation == "!") {
            iss >> word >> filePath;
            if (word == "Save") {
                dict.save(filePath);
            } else if (word == "Load") {
                dict.load(filePath);
            }
        } else {
            dict.find(operation);
        }
    }

    return 0;
}
