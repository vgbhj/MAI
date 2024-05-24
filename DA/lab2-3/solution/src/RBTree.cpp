#include "RBTree.h"

RBTree::RBTree() : root(nullptr) {}

RBTree::~RBTree() {
    clear();
}

void RBTree::rotateLeft(Node* &root, Node* &pt) {
    Node* pt_right = pt->right;
    pt->right = pt_right->left;
    if (pt->right != nullptr) pt->right->parent = pt;
    pt_right->parent = pt->parent;
    if (pt->parent == nullptr) root = pt_right;
    else if (pt == pt->parent->left) pt->parent->left = pt_right;
    else pt->parent->right = pt_right;
    pt_right->left = pt;
    pt->parent = pt_right;
}

void RBTree::rotateRight(Node* &root, Node* &pt) {
    Node* pt_left = pt->left;
    pt->left = pt_left->right;
    if (pt->left != nullptr) pt->left->parent = pt;
    pt_left->parent = pt->parent;
    if (pt->parent == nullptr) root = pt_left;
    else if (pt == pt->parent->left) pt->parent->left = pt_left;
    else pt->parent->right = pt_left;
    pt_left->right = pt;
    pt->parent = pt_left;
}

void RBTree::fixInsertRBTree(Node* &root, Node* &pt) {
    Node* parent_pt = nullptr;
    Node* grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (parent_pt == grand_parent_pt->left) {
            Node* uncle_pt = grand_parent_pt->right;
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->right) {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateRight(root, grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        } else {
            Node* uncle_pt = grand_parent_pt->left;
            if (uncle_pt != nullptr && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            } else {
                if (pt == parent_pt->left) {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateLeft(root, grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    root->color = BLACK;
}

RBTree::Node* RBTree::BSTInsert(Node* root, Node* pt) {
    if (root == nullptr) return pt;
    if (pt->key < root->key) {
        root->left = BSTInsert(root->left, pt);
        root->left->parent = root;
    } else if (pt->key > root->key) {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }
    return root;
}

RBTree::Node* RBTree::searchHelper(Node* root, std::string key) {
    if (root == nullptr || root->key == key) return root;
    if (root->key < key) return searchHelper(root->right, key);
    return searchHelper(root->left, key);
}

void RBTree::deleteNodeHelper(Node* &root, std::string key) {
    // Реализация удаления узла в красно-черном дереве
    // Эта функция должна быть реализована, чтобы поддерживать удаление
}

void RBTree::clearHelper(Node* &node) {
    if (node == nullptr) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
    node = nullptr;
}

void RBTree::insertValue(std::string key, unsigned long long value) {
    key = toLower(key);
    Node* pt = new Node(key, value);
    root = BSTInsert(root, pt);
    fixInsertRBTree(root, pt);
}

void RBTree::deleteValue(std::string key) {
    key = toLower(key);
    deleteNodeHelper(root, key);
}

unsigned long long RBTree::searchValue(std::string key) {
    key = toLower(key);
    Node* node = searchHelper(root, key);
    if (node == nullptr) return static_cast<unsigned long long>(-1);
    return node->value;
}

void RBTree::clear() {
    clearHelper(root);
    root = nullptr;
}

std::string RBTree::toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void RBTree::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "ERROR: Could not open file for writing" << std::endl;
        return;
    }
    saveHelper(outFile, root);
    outFile.close();
}

void RBTree::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "ERROR: Could not open file for reading" << std::endl;
        return;
    }
    clear();
    loadHelper(inFile);
    inFile.close();
}

void RBTree::saveHelper(std::ofstream &outFile, Node* node) {
    if (node == nullptr) {
        outFile.put(0);
        return;
    }
    outFile.put(1);
    size_t keySize = node->key.size();
    outFile.write(reinterpret_cast<char*>(&keySize), sizeof(keySize));
    outFile.write(node->key.data(), keySize);
    outFile.write(reinterpret_cast<char*>(&node->value), sizeof(node->value));
    saveHelper(outFile, node->left);
    saveHelper(outFile, node->right);
}

void RBTree::loadHelper(std::ifstream &inFile) {
    char flag;
    inFile.get(flag);
    if (!inFile || flag == 0) return;
    size_t keySize;
    inFile.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
    std::string key(keySize, '\0');
    inFile.read(&key[0], keySize);
    unsigned long long value;
    inFile.read(reinterpret_cast<char*>(&value), sizeof(value));
    insertValue(key, value);
    loadHelper(inFile);
    loadHelper(inFile);
}
