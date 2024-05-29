#include <bits/stdc++.h>
#include "RBTree.h"

using namespace std;

class Dictionary {
    RBTree tree;
public:
    void processCommand(const string& command) {
        if (command.rfind("+", 0) == 0) {
            size_t spacePos = command.find(' ', 2);
            if (spacePos == string::npos) {
                cerr << "Invalid command format" << endl;
                return;
            }
            string word = command.substr(2, spacePos - 2);
            string valueStr = command.substr(spacePos + 1);
            try {
                unsigned long long value = stoull(valueStr);
                if (tree.searchBool(word) != false) {
                    cout << "Exist" << endl;
                } else {
                    tree.insertValue(word, value);
                    cout << "OK" << endl;
                }
            } catch (const invalid_argument& e) {
                cerr << "Invalid number format: " << valueStr << endl;
            } catch (const out_of_range& e) {
                cerr << "Number out of range: " << valueStr << endl;
            }
        } else if (command.rfind("-", 0) == 0) {
            string word = command.substr(2);
            if (tree.searchBool(word) == false) {
                cout << "NoSuchWord" << endl;
            } else {
                tree.deleteValue(word);
                cout << "OK" << endl;
            }
        } else if (command.rfind("!", 0) == 0) {
            size_t spacePos = command.find(' ', 2);
            if (spacePos == string::npos) {
                cerr << "Invalid command format" << endl;
                return;
            }
            string action = command.substr(2, spacePos - 2);
            string path = command.substr(spacePos + 1);
            if (action == "Save") {
                tree.saveToFile(path);
                cout << "OK" << endl;
            } else if (action == "Load") {
                tree.loadFromFile(path);
                cout << "OK" << endl;
            } else {
                cerr << "Invalid action: " << action << endl;
            }
        } else {
            string word = command;
            unsigned long long value = tree.search(word);
            bool flag = tree.searchBool(word);
            if (flag == false) {
                cout << "NoSuchWord" << endl;
            } else {
                cout << "OK: " << value << endl;
            }
        }
    }
};


int main() {
    // cout << static_cast<unsigned long long>(-1);
    Dictionary dict;
    string line;
    while (getline(cin, line)) {
        dict.processCommand(line);
    }
    return 0;
}