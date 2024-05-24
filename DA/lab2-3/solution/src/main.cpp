#include <iostream>
#include <string>
#include "RBTree.h"

class Dictionary {
    RBTree tree;
public:
    void processCommand(const std::string& command) {
        if (command.rfind("+", 0) == 0) {
            size_t spacePos = command.find(' ', 2);
            if (spacePos == std::string::npos) {
                std::cerr << "Invalid command format" << std::endl;
                return;
            }
            std::string word = command.substr(2, spacePos - 2);
            std::string valueStr = command.substr(spacePos + 1);
            try {
                unsigned long long value = std::stoull(valueStr);
                if (tree.searchValue(word) != static_cast<unsigned long long>(-1)) {
                    std::cout << "Exist" << std::endl;
                } else {
                    tree.insertValue(word, value);
                    std::cout << "OK" << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid number format: " << valueStr << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Number out of range: " << valueStr << std::endl;
            }
        } else if (command.rfind("-", 0) == 0) {
            std::string word = command.substr(2);
            if (tree.searchValue(word) == static_cast<unsigned long long>(-1)) {
                std::cout << "NoSuchWord" << std::endl;
            } else {
                tree.deleteValue(word);
                std::cout << "OK" << std::endl;
            }
        } else if (command.rfind("!", 0) == 0) {
            size_t spacePos = command.find(' ', 2);
            if (spacePos == std::string::npos) {
                std::cerr << "Invalid command format" << std::endl;
                return;
            }
            std::string action = command.substr(2, spacePos - 2);
            std::string path = command.substr(spacePos + 1);
            if (action == "Save") {
                tree.saveToFile(path);
                std::cout << "OK" << std::endl;
            } else if (action == "Load") {
                tree.loadFromFile(path);
                std::cout << "OK" << std::endl;
            } else {
                std::cerr << "Invalid action: " << action << std::endl;
            }
        } else {
            std::string word = command;
            unsigned long long value = tree.searchValue(word);
            if (value == static_cast<unsigned long long>(-1)) {
                std::cout << "NoSuchWord" << std::endl;
            } else {
                std::cout << "OK: " << value << std::endl;
            }
        }
    }
};

int main() {
    Dictionary dict;
    std::string line;
    while (std::getline(std::cin, line)) {
        dict.processCommand(line);
    }
    return 0;
}
