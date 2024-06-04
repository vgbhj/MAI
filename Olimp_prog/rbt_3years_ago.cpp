#include <iostream>

namespace NDictionary {
    const int MAX_VALUE_SIZE = 257;
    const int RESULT_LESS = 1;
    const int RESULT_MORE = 2;
    const int RESULT_EQUAL = 3;
    const int ZERO_CONST = 0;

    typedef unsigned long long TUll;
    enum TColor { BLACK = 1, RED = 2 };
    enum TResult { ZERO = 0, OK = 1, Exist = 2, NoSuchWord = 3, Specific = 4 };

    struct TWord {
        char mword[MAX_VALUE_SIZE];
        int wordS;
        TUll mkey;
    };

    struct TNode {
        TNode *left;
        TNode *right;
        TNode *parent;
        TColor color;
        TWord word;
    };

    static int CompareWords(const TWord &n1, const TWord &n2) {
        if(n1.wordS != n2.wordS) {
            if(n1.wordS < n2.wordS) {
                return  RESULT_LESS;
            }
            return RESULT_MORE;
        }

        for (int i = 0; i < MAX_VALUE_SIZE; ++i) {
            if(n1.mword[i] == 0 && n2.mword[i] == 0) {
                return RESULT_EQUAL;
            }
            if(n1.mword[i] == 0 && n2.mword[i] != 0) {
                return RESULT_LESS;
            }
            if(n1.mword[i] != 0 && n2.mword[i] == 0) {
                return RESULT_MORE;
            }
            if(tolower(n1.mword[i]) - tolower(n2.mword[i]) == ZERO_CONST) {
                continue;
            }
            if (tolower(n1.mword[i]) - tolower(n2.mword[i]) < ZERO_CONST) { //a - b < 0    <true
                return RESULT_LESS;
            }
            else { //c - b > 0    <false
                return RESULT_MORE;
            }
        }
        return RESULT_EQUAL;
    }

    class TRBTree
            {

            public:
                TNode *mroot;
                TResult mres;

                TRBTree() {
                    NIL = new TNode();
                    NIL->color = BLACK;
                    mroot = NIL;
                    mres = ZERO;
                }

                ~TRBTree() {
                    if(mroot != NIL) {
                        DeleteTree(mroot);
                    }
                    delete NIL;
                }

                TNode *FindNode(TWord &word) {
                    if (mroot == NIL || mroot == nullptr) {
                        mres = NoSuchWord;
                        return nullptr;
                    }
                    TNode *runner = mroot;
                    while (runner != NIL) {
                        int rescomp = CompareWords(word, runner->word);
                        if (rescomp == RESULT_LESS) { 
                            runner = runner->left;
                        }
                        else if (rescomp == RESULT_MORE)
                        {
                            runner = runner->right;
                        }
                        else {
                            return runner;
                        }
                    }
                    mres = NoSuchWord;
                    return nullptr;
                }

                //insert + (LeftRotate + RightRotate + InsertFixup)


                bool CheckNode(TNode *runner, TWord &data) {
                    if(runner == NIL) {
                        return false;
                    }
                    int rescomp = CompareWords(runner->word, data);
                    if(rescomp == RESULT_LESS) {
                        return CheckNode(runner->left, data);
                    }
                    else if(rescomp == RESULT_MORE) {
                        return CheckNode(runner->right, data);
                    }
                    else {
                        return true;
                    }
                }

                TUll CheckNodeKey(TNode *runner, TWord &data) {
                    if(runner == NIL) {
                        mres = NoSuchWord;
                        return 0;
                    }
                    int rescomp = CompareWords(runner->word, data);
                    if(rescomp == RESULT_LESS) {
                        return CheckNode(runner->left, data);
                    }
                    else if(rescomp == RESULT_MORE) {
                        return CheckNode(runner->right, data);
                    }
                    else {
                        mres = Specific;
                        return runner->word.mkey;
                    }
                }

                void Insert(TWord &data) {
                    TNode *node = new TNode();
                    node->parent = nullptr;
                    node->word = data;
                    node->left = NIL;
                    node->right = NIL;
                    node->color = RED;

                    TNode *y = nullptr;
                    TNode *x = mroot;

                    while (x != NIL) {
                        y = x;
                        int rescomp = CompareWords(node->word, x->word);
                        if(rescomp == RESULT_EQUAL) {
                            delete node;
                            mres = Exist;
                            return;
                        }
                        if (rescomp == RESULT_LESS) {
                            x = x->left;
                        } else {
                            x = x->right;
                        }
                    }

                    node->parent = y;
                    if (y == nullptr) {
                        mroot = node;
                    } else if (CompareWords(node->word, y->word) == RESULT_LESS) {
                        y->left = node;
                    } else {
                        y->right = node;
                    }

                    if (node->parent == nullptr) {
                        node->color = BLACK;
                        mres = OK;
                        return;
                    }

                    if (node->parent->parent == nullptr) {
                        mres = OK;
                        return;
                    }

                    InsertFixup(node);
                    mres = OK;
                }


                void Delete(TNode *node, TWord &data) {
                    TNode *z = NIL;
                    TNode *x, *y;
                    while (node != NIL) {
                        int rescomp = CompareWords(node->word, data);
                        if (rescomp == RESULT_EQUAL) {
                            z = node;
                        }
                        if (rescomp == RESULT_MORE) {
                            node = node->left;
                        } else {
                            node = node->right;
                        }
                    }

                    if (z == NIL) {
                        mres = NoSuchWord;
                        return;
                    }

                    y = z;
                    TColor y_original_color = y->color;
                    if (z->left == NIL) {
                        x = z->right;
                        Transplant(z, z->right);
                    } else if (z->right == NIL) {
                        x = z->left;
                        Transplant(z, z->left);
                    } else {
                        y = TreeMinimum(z->right);
                        y_original_color = y->color;
                        x = y->right;
                        if (y->parent == z) {
                            x->parent = y;
                        } else {
                            Transplant(y, y->right);
                            y->right = z->right;
                            y->right->parent = y;
                        }

                        Transplant(z, y);
                        y->left = z->left;
                        y->left->parent = y;
                        y->color = z->color;
                    }
                    delete z;
                    if (y_original_color == BLACK) {
                        DeleteFixup(x);
                    }
                    mres = OK;
                }

                void OutputB(TNode *runner) {
                    if (mroot == NIL) {
                        return;
                    }
                    std::cout << "ROOT   " << runner->word.mword << " - " << runner->word.mkey << std::endl;
                    if (runner->left != NIL) {
                        std::cout << "  ";
                        OutputB(runner->left);
                    }
                    if (runner->right != NIL) {
                        std::cout << "	  ";
                        OutputB(runner->right);
                    }
                }

                TUll KeyReturner(TWord &word) {
                    if (mroot == NIL) {
                        mres = NoSuchWord;
                        return ZERO_CONST;
                    }
                    TNode *runner = mroot;
                    while (runner != NIL) {
                        int rescomp = CompareWords(word, runner->word);
                        if (rescomp == RESULT_LESS) {
                            runner = runner->left;
                        }
                        else if (rescomp == RESULT_MORE)
                        {
                            runner = runner->right;
                        }
                        else {
                            mres = Specific;
                            return runner->word.mkey;
                        }
                    }
                    mres = NoSuchWord;
                    return ZERO_CONST;
                }

                //save & load
                void SaveData(TNode *runner, FILE* file) {
                    if(runner == NIL) {
                        int stop = -1;
                        fwrite(&stop, sizeof(int), 1, file);
                        return;
                    }
                    else {
                        fwrite(&runner->word.wordS, sizeof(int), 1, file);
                        // SyscallParam error valgrind ??????????!!!!!!!
                        fwrite(&runner->word.mword, sizeof(char),  MAX_VALUE_SIZE, file);
                        fwrite(&runner->word.mkey, sizeof(TUll), 1, file);
                        if(runner->color == BLACK) {
                            char c = 'b';
                            fwrite(&c, sizeof(char), 1, file);
                        }
                        else {
                            char c = 'r';
                            fwrite(&c, sizeof(char), 1, file);
                        }
                        SaveData(runner->right, file);
                        SaveData(runner->left, file);
                    }

                }

                TNode* LoadData(FILE* file) {
                    TWord checker;
                    TColor color;
                    unsigned  long jj = 0;
                    jj = fread(&checker.wordS, sizeof(int), 1, file);
                    if(checker.wordS == -1) {
                        return NIL;
                    }
                    jj = fread(&checker.mword, sizeof(char), MAX_VALUE_SIZE, file);
                    jj = fread(&checker.mkey, sizeof(TUll), 1, file);
                    char c;
                    jj = fread(&c, sizeof(char), 1, file);
                    if(c == 'b') {
                        color = BLACK;
                    }
                    else {
                        color = RED;
                    }
                    TNode *tmp = new TNode();
                    tmp->word = checker;
                    tmp->color = color;
                    tmp->right = LoadData(file);
                    tmp->left = LoadData(file);
                    if(tmp->left != NIL) {
                        tmp->left->parent = tmp;
                    }
                    if(tmp->right != NIL) {
                        tmp->right->parent = tmp;
                    }
                    if(jj == -5) {
                        printf("ERROR");
                    }
                    return tmp;
                }

                void DeleteTree(TNode *runner) {
                    if(runner->left != NIL) {
                        DeleteTree(runner->left);
                    }
                    if(runner->right != NIL) {
                        DeleteTree(runner->right);
                    }
                    delete runner;
                }

                bool IsEmpty() {
                    if(mroot == NIL) {
                        return true;
                    }
                    return false;
                }

            private:

                TNode *NIL;

                //Additional insert functions
                void LeftRotate(TNode *x) { // O(1)
                    TNode *y = x->right;
                    x->right = y->left;

                    if (y->left != NIL) {
                        y->left->parent = x;
                    }

                    y->parent = x->parent;
                    if (x->parent == nullptr) {
                        mroot = y;
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

                void RightRotate(TNode *x) { // O(1)
                    TNode *y = x->left;
                    x->left = y->right;

                    if (y->right != NIL) {
                        y->right->parent = x;
                    }

                    y->parent = x->parent;
                    if (x->parent == nullptr) {
                        mroot = y;
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

                void InsertFixup(TNode *z) {
                    while (z->parent != nullptr && z->parent->color == RED) {
                        if (z->parent == z->parent->parent->left) {
                            TNode *y = z->parent->parent->right;
                            if (y->color == RED) {
                                z->parent->color = BLACK;
                                y->color = BLACK;
                                z->parent->parent->color = RED;
                                z = z->parent->parent;
                            }
                            else {
                                if (z == z->parent->right) {
                                    z = z->parent;
                                    LeftRotate(z);
                                }
                                z->parent->color = BLACK;
                                z->parent->parent->color = RED;
                                RightRotate(z->parent->parent);
                            }
                        }
                        else {
                            TNode *y = z->parent->parent->left;
                            if (y->color == RED) {
                                z->parent->color = BLACK;
                                y->color = BLACK;
                                z->parent->parent->color = RED;
                                z = z->parent->parent;
                            }
                            else {
                                if (z == z->parent->left) {
                                    z = z->parent;
                                    RightRotate(z);
                                }
                                z->parent->color = BLACK;
                                z->parent->parent->color = RED;
                                LeftRotate(z->parent->parent);
                            }
                        }
                    }
                    mroot->color = BLACK;
                }

                //Additional delete functions

                void Transplant(TNode *u, TNode *v) { //change u to v in every condition
                    if (u->parent == nullptr) {
                        mroot = v;
                    } else if (u == u->parent->left) {
                        u->parent->left = v;
                    }
                    else {
                        u->parent->right = v;
                    }
                    v->parent = u->parent;
                }

                TNode *TreeMinimum(TNode *start) {
                    while (start->left != NIL) {
                        start = start->left;
                    }
                    return start;
                }

                void DeleteFixup(TNode *x) {
                    TNode *s;
                    while (x != mroot && x->color == BLACK) {
                        if (x == x->parent->left) {
                            s = x->parent->right;
                            if (s->color == RED) {
                                s->color = BLACK;
                                x->parent->color = RED;
                                LeftRotate(x->parent);
                                s = x->parent->right;
                            }

                            if (s->left->color == BLACK && s->right->color == BLACK) {
                                s->color = RED;
                                x = x->parent;
                            } else {
                                if (s->right->color == BLACK) {
                                    s->left->color = BLACK;
                                    s->color = RED;
                                    RightRotate(s);
                                    s = x->parent->right;
                                }

                                s->color = x->parent->color;
                                x->parent->color = BLACK;
                                s->right->color = BLACK;
                                LeftRotate(x->parent);
                                x = mroot;
                            }
                        } else {
                            s = x->parent->left;
                            if (s->color == RED) {
                                s->color = BLACK;
                                x->parent->color = RED;
                                RightRotate(x->parent);
                                s = x->parent->left;
                            }

                            if (s->right->color == BLACK && s->left->color == BLACK) {
                                s->color = RED;
                                x = x->parent;
                            } else {
                                if (s->left->color == BLACK) {
                                    s->right->color = BLACK;
                                    s->color = RED;
                                    LeftRotate(s);
                                    s = x->parent->left;
                                }

                                s->color = x->parent->color;
                                x->parent->color = BLACK;
                                s->left->color = BLACK;
                                RightRotate(x->parent);
                                x = mroot;
                            }
                        }
                    }
                    x->color = BLACK;
                }

            };
}





using namespace NDictionary;

void ShowResult(TResult &res) {
    switch (res)
    {
        case(OK):
            printf("OK\n");
            break;
            case(Exist):
                printf("Exist\n");
                break;
                case(NoSuchWord):
                    printf("NoSuchWord\n");
                    break;
                    case(Specific):
                        //printf("Special\n");
                        break;
                    default:
                        break;
    }
}

int main()
{
    TRBTree tree;
    char switcher[MAX_VALUE_SIZE];
    int resscan = 0;

    while (scanf("%s", switcher) != EOF) {
        TWord tmp;
        tmp.mkey = 0;
        tree.mres = ZERO;
        if (switcher[0] == '+') { //add
            for (int i = 0; i < MAX_VALUE_SIZE; ++i) {
                tmp.mword[i] = 0;
            }
            resscan = scanf("%s %llu", tmp.mword, &tmp.mkey);
            for(int i = 0; i < MAX_VALUE_SIZE; ++i) {
                if(tmp.mword[i] == 0) {
                    tmp.wordS = i;
                    break;
                }
            }
            tree.Insert(tmp);
        }
        else {
            if (switcher[0] == '-') { //delete
                for (int i = 0; i < MAX_VALUE_SIZE; ++i) {
                    tmp.mword[i] = 0;
                }
                resscan = scanf("%s", tmp.mword);
                for(int i = 0; i < MAX_VALUE_SIZE; ++i) {
                    if(tmp.mword[i] == 0) {
                        tmp.wordS = i;
                        break;
                    }
                }
                tree.Delete(tree.mroot, tmp);

            }
            else {
                if(switcher[0] == '!') { //save&load
                    resscan = scanf("%s", switcher);
                    if (switcher[0] == 'L') {
                        resscan = scanf("%s", switcher);
                        FILE* streamLoad= nullptr;
                        streamLoad = fopen(switcher, "rb");
                        if (streamLoad == nullptr) {
                            printf("ERROR: error opening file to read!\n");
                        } else {
                            if(!tree.IsEmpty()) {
                                tree.DeleteTree(tree.mroot);
                            }
                            tree.mroot = tree.LoadData(streamLoad);
                            printf("OK\n");
                            fclose(streamLoad);
                        }
                    } else {
                        resscan = scanf("%s", switcher);
                        FILE* streamSave = nullptr;
                        streamSave = fopen(switcher, "wb");
                        if (streamSave == nullptr) {
                            printf("ERROR: error opening file to write!\n");
                        } else {
                            tree.SaveData(tree.mroot, streamSave);
                            printf("OK\n");
                            fclose(streamSave);
                        }
                    }
                }
                else {
                    for (int i = 0; i < MAX_VALUE_SIZE; ++i) {
                        tmp.mword[i] = switcher[i];
                    }
                    for(int i = 0; i < MAX_VALUE_SIZE; ++i) {
                        if(tmp.mword[i] == 0) {
                            tmp.wordS = i;
                            break;
                        }
                    }
                    tmp.mkey = tree.KeyReturner(tmp);
                    if(tree.mres == Specific) {
                        printf("OK: %llu \n", tmp.mkey);
                    } else {
                        tree.mres = NoSuchWord;
                    }
                }
            }
        }
        ShowResult(tree.mres);
    }


    if(resscan == -5) {
        printf("ERROR");
    }
}