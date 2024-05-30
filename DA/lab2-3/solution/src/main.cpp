#include <bits/stdc++.h>
#include "RBTree.h"

using namespace std;

int main() {
    std::ios::sync_with_stdio(false);
    char comm[50];
    char key[257] = {'\0'};
    unsigned long long value;

    RBTree tree;

    while(cin >> comm) {
        if (comm[0] == '+') {
            cin >> key >> value;
            if(tree.searchBool(key) == false) {
                tree.insertValue(key,value);
                puts("OK");
            } else {
                puts("Exist");
            }
        } else if (comm[0] == '-') {
            cin >> key;
            if(tree.searchBool(key)){
                tree.deleteValue(key);
                puts("OK");
            } else {
                puts("NoSuchWord");
            }
        } else if (comm[0] == '!') {
            cin >> comm >> path;
            if (comm[0] == 'S') {
                if(tree.saveToFile(path)) {
                    puts("OK");
                } else {
                    puts("ERROR");
                }
            } else if (comm[0] == 'L') {
                if(tree.loadFromFile(path)) {
                    puts("OK");
                } else {
                    puts("ERROR");
                }
            }
        } else {
            try {
                value = tree.search(comm);
                printf("OK: %llu\n", value);
            } catch (...) {
                puts("NoSuchWord");
            }
        }
    }
    return 0;
}