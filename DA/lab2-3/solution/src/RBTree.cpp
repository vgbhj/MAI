#include "RBTree.h"

RedBlackTree::RedBlackTree(){
    NIL = new Node("0", 0);
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL;
    root = NIL;
}

RedBlackTree::~RedBlackTree() {
    clear();
}

void RedBlackTree::leftRotate(Node* x){
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
void RedBlackTree::rightRotate(Node* x){
    Node* y = x->left;
    x->left = y->right;
    if (y->right != NIL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void RedBlackTree::fixInsert(Node* k){
    while (k != root && k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) {
            Node* u = k->parent->parent->right; // uncle
            if (u->color == RED) {
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        }
        else {
            Node* u = k->parent->parent->left; // uncle
            if (u->color == RED) {
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            }
            else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void RedBlackTree::inorderHelper(Node* node)
{
    if (node != NIL && node != NULL) {
        inorderHelper(node->left);
        std::cout << node->key << " " << node->value << " ";
        inorderHelper(node->right);
    }
}

RedBlackTree::Node* RedBlackTree::searchHelper(Node* node, std::string key)
{
    if (node == NIL || key == node->key) {
        return node;
    }
    if (key < node->key) {
        return searchHelper(node->left, key);
    }
    return searchHelper(node->right, key);
}


void RedBlackTree::clear() {
    clearHelper(root);
    root = nullptr;
}

void RedBlackTree::clearHelper(Node* &node) {
    if (node == nullptr) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
    node = nullptr;
}

std::string RedBlackTree::toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

void RedBlackTree::insert(std::string k, unsigned long long v)
{

    k = toLower(k);
    Node* new_node = new Node(k ,v);
    new_node->left = NIL;
    new_node->right = NIL;

    Node* parent = nullptr;
    Node* current = root;

    while (current != NIL) {
        parent = current;
        if (new_node->key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    new_node->parent = parent;

    if (parent == nullptr) {
        root = new_node;
    }
    else if (new_node->key < parent->key) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }

    if (new_node->parent == nullptr) {
        new_node->color = BLACK;
        return;
    }

    if (new_node->parent->parent == nullptr) {
        return;
    }

    fixInsert(new_node);
    if(new_node->parent != nullptr){
        // std::cout << new_node->parent->value;
    }
}

void RedBlackTree::swapColors(Node *x1, Node *x2){
    RedBlackTree::Color temp;
    temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
}

void RedBlackTree::swapValues(Node *u, Node *v) {
    unsigned long long temp;
    std::string temp_str;
    temp_str = u->key;
    temp = u->value;
    u->value = v->value;
    u->key = v->key;
    v->value = temp;
    v->key = temp_str;
}

void RedBlackTree::fixRedRed(Node *x) {
    // if x is root color it black and return
    if (x == root) {
      x->color = BLACK;
      return;
    }
 
    // initialize parent, grandparent, uncle
    Node *parent = x->parent, *grandparent = parent->parent,
         *uncle = x->uncle();
 
    if (parent->color != BLACK) {
      if (uncle != NULL && uncle->color == RED) {
        // uncle red, perform recoloring and recurse
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        fixRedRed(grandparent);
      } else {
        // Else perform LR, LL, RL, RR
        if (parent->isOnLeft()) {
          if (x->isOnLeft()) {
            // for left right
            swapColors(parent, grandparent);
          } else {
            leftRotate(parent);
            swapColors(x, grandparent);
          }
          // for left left and left right
          rightRotate(grandparent);
        } else {
          if (x->isOnLeft()) {
            // for right left
            rightRotate(parent);
            swapColors(x, grandparent);
          } else {
            swapColors(parent, grandparent);
          }
 
          // for right right and right left
          leftRotate(grandparent);
        }
      }
    }
  }

RedBlackTree::Node *RedBlackTree::successor(Node *x) {
    Node *temp = x; 
    // std::cout << temp->key << ' ';
    while (temp->left->key != "0"){
        temp = temp->left;
    //     // std::cout << temp->value << ' ' << temp->key << '\n';
    //     // std::cout << temp->left->value << ' ' << temp->left->key << '\n';
    //     // std::cout << (temp->left != NULL) << ' ' << (temp->left->key != "0") << '\n';

    }
 
    return temp;
}
 
  // find node that replaces a deleted node in BST
RedBlackTree::Node *RedBlackTree::BSTreplace(RedBlackTree::Node *x) {

    
    // when node have 2 children
    if (x->left->key != "0" and x->right->key != "0")
      return RedBlackTree::successor(x->right);
 
    // when leaf
    if (x->left->key == "0" and x->right->key == "0")
      return NULL;
 
    // when single child
    if (x->left->key != "0")
      return x->left;
    else
      return x->right;
      
  }
 
  // deletes the given node
void RedBlackTree::deleteNode(Node *v) {
    Node *u = BSTreplace(v);
 
    // True when u and v are both black
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    Node *parent = v->parent;
 
    if (u == NULL) {
      // u is NULL therefore v is leaf
      if (v == root) {
        // v is root, making root null
        root = NULL;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          fixDoubleBlack(v);
        } else {
          // u or v is red
          if (v->sibling() != NULL)
            // sibling is not null, make it red"
            v->sibling()->color = RED;
        }
 
        // delete v from the tree
        if (v->isOnLeft()) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }
 
    if (v->left == NULL or v->right == NULL) {
      // v has 1 child
      if (v == root) {
        // v is root, assign the value of u to v, and delete u
        v->value = u->value;
        v->key = u->key;
        v->left = v->right = NULL;
        delete u;
      } else {
        // Detach v from tree and move u up
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->color = BLACK;
        }
      }
      return;
    }
 
    // v has 2 children, swap values with successor and recurse
    swapValues(u, v);
    deleteNode(u);
  }
 
  void RedBlackTree::fixDoubleBlack(Node *x) {
    if (x == root)
      // Reached root
      return;
 
    Node *sibling = x->sibling(), *parent = x->parent;
    if (sibling == NULL) {
      // No sibling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        // Sibling red
        parent->color = RED;
        sibling->color = BLACK;
        if (sibling->isOnLeft()) {
          // left case
          rightRotate(parent);
        } else {
          // right case
          leftRotate(parent);
        }
        fixDoubleBlack(x);
      } else {
        // Sibling black
        if (sibling->hasRedChild()) {
          // at least 1 red children
          if (sibling->left != NULL and sibling->left->color == RED) {
            if (sibling->isOnLeft()) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rightRotate(parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rightRotate(sibling);
              leftRotate(parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              // left right
              sibling->right->color = parent->color;
              leftRotate(sibling);
              rightRotate(parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              leftRotate(parent);
            }
          }
          parent->color = BLACK;
        } else {
          // 2 black children
          sibling->color = RED;
          if (parent->color == BLACK)
            fixDoubleBlack(parent);
          else
            parent->color = BLACK;
        }
      }
    }
  }