#include <bits/stdc++.h>
#include "RBTree.h"

using namespace std;

int main() {
    // ios::sync_with_stdio(false);
    // cin.tie(0);
    RBTree tree;
    string command, key, path, answer;
    ull value;
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
                tree.get(key);
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
