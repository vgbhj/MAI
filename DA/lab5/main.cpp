#include <bits/stdc++.h>
using namespace std;

struct SuffixTree {
    struct Node {
        int l, r; // Индексы на строке: [l, r)
        map<char, Node*> next; // Переходы по символам
        Node* link; // Суффиксная ссылка
        int depth; // Глубина узла
        Node(int l = 0, int r = 0) : l(l), r(r), link(nullptr), depth(0) {}
        int length(int end) const { return min(r, end + 1) - l; } // Длина ребра
    };

    int findEdge(Node* node, char c) {
        if (node->next.count(c)) {
            return node->next[c]->l;
        }
        return -1;
    }



    string s;
    Node* root; // Корень дерева
    Node* active_node; // Текущая активная вершина
    int active_edge, active_length; // Активное ребро и длина
    int remainder; // Оставшиеся суффиксы
    int pos;
    int size;

    SuffixTree() : root(new Node()), active_node(root), active_edge(0), active_length(0), remainder(0), pos(-1) {}

    ~SuffixTree() { delete_tree(root); }

    void delete_tree(Node* node) {
        for (auto& p : node->next) delete_tree(p.second);
        delete node;
    }

    void add_char(char c) {
    s += c;
    pos++;
    remainder++;
    Node* last_new_node = nullptr;

    while (remainder > 0) {
        if (active_length == 0) active_edge = pos;

        char current_char = s[active_edge];
        if (!active_node->next.count(current_char)) {
            // Если ребро не существует, создаем новое
            Node* leaf = new Node(pos, size);
            active_node->next[current_char] = leaf;

            if (last_new_node) {
                last_new_node->link = active_node;
                last_new_node = nullptr;
            }
        } else {
            Node* next = active_node->next[current_char];
            if (walk_down(next)) continue;

            if (s[next->l + active_length] == c) {
                active_length++;
                if (last_new_node) {
                    last_new_node->link = active_node;
                    last_new_node = nullptr;
                }
                break;
            }

            Node* split = new Node(next->l, next->l + active_length);
            active_node->next[current_char] = split;
            Node* leaf = new Node(pos, size);
            split->next[c] = leaf;
            next->l += active_length;
            split->next[s[next->l]] = next;

            if (last_new_node) last_new_node->link = split;
            last_new_node = split;
        }

        remainder--;
        if (active_node == root && active_length > 0) {
            active_length--;
            active_edge = pos - remainder + 1;
        } else {
            active_node = (active_node->link ? active_node->link : root);
        }
    }
}


    bool walk_down(Node* next) {
        if (active_length >= next->length(pos)) {
            active_edge += next->length(pos);
            active_length -= next->length(pos);
            active_node = next;
            return true;
        }
        return false;
    }

    void build(const string& text) {
        s = "";
        size = text.size();
        for (char c : text) {
            add_char(c);
        }
    }

    void print(Node* node, int depth = 0) {
        if (node == nullptr) return;

        for (auto& p : node->next) {
            cout << string(depth * 2, ' ') << "[" << s.substr(p.second->l, p.second->length(pos)) << "]\n";
            print(p.second, depth + 1);
        }
    }

    void print() {
        print(root);
    }

    // int search(const string& pattern) {
    //     Node* current = root;
    //     int i = 0; // Индекс в строке `pattern`
    //     int length = pattern.length();
    //     int matched = 0;

    //     while (i < length) {
    //         auto it = current->next.find(pattern[i]);
    //         if (it == current->next.end()) break; // Перехода нет, строка не найдена

    //         Node* next = it->second;
    //         int edge_len = next->length(pos);

    //         for (int j = 0; j < edge_len && i < length; ++j) {
    //             if (s[next->l + j] != pattern[i]) return matched; // Несовпадение символа
    //             i++;
    //             matched++;
    //         }

    //         current = next;
    //     }

    //     return matched; // Возвращаем количество совпавших символов
    // }

    std::vector<size_t> findMatches(const std::string& query) {
    size_t queryLength = query.size();
    std::vector<size_t> result(queryLength, 0);
    Node* currentNode = root;  // Начинаем с корня
    int currentEdge = -1;
    int commonLength = 0;
    int currentMatchLength = 0;
    std::stack<Node*> nodeStack;
    nodeStack.push(currentNode);

    for (size_t i = 0; i < queryLength; ++i) {
        int nextNode = currentNode->link; // Используем суффиксные ссылки для поиска предыдущих узлов

        while (nextNode == nullptr && !nodeStack.empty()) {
            nextNode = nodeStack.top()->link;
            nodeStack.pop();
        }

        currentNode = nextNode;
        nodeStack.push(currentNode);

        currentMatchLength = std::max(0, commonLength + currentMatchLength - 1 - currentNode->depth);
        commonLength = currentNode->depth;

        currentEdge = -1;
        while (currentMatchLength >= 0) {
            // Переход по ребрам дерева
            int nextEdge = findEdge(currentNode, query[i + commonLength]);

            if (nextEdge != -1) {
                int edgeLength = currentNode->next[query[i + commonLength]]->length(pos);
                if (currentMatchLength >= edgeLength) {
                    currentMatchLength -= edgeLength;
                    commonLength += edgeLength;
                    currentNode = currentNode->next[query[i + commonLength]];
                    nextEdge = -1;
                } else {
                    currentEdge = nextEdge;
                    break;
                }
            } else {
                break;
            }
        }

        while (currentEdge != -1 && i + commonLength + currentMatchLength < queryLength && query[i + commonLength + currentMatchLength] == s[adjacencyList[currentNode][currentEdge].start + currentMatchLength]) {
            ++currentMatchLength;

            if (adjacencyList[currentNode][currentEdge].start + currentMatchLength == *adjacencyList[currentNode][currentEdge].end) {
                int nextNode = adjacencyList[currentNode][currentEdge].destination;
                int nextEdge = findEdge(nextNode, query[i + commonLength + currentMatchLength]);
                int edgeLength = currentMatchLength;
                commonLength += edgeLength;
                currentMatchLength = 0;
                currentNode = nextNode;
                nodeStack.push(currentNode);
                currentEdge = nextEdge;
            }
        }
        result[i] = commonLength + currentMatchLength;
    }

    return result;
}




    
    // match statistic
    vector<int> ms_build(string& text){
        vector<int> ms(text.length());
        Node* current = root;
        int index = 0;
        while (index < text.length()){

        }
    }

};

int main() {
    string str1, str2;
    getline(std::cin, str1);
    getline(std::cin, str2);
    
    string text, pattern;

    // Определение большей и меньшей строки
    if (str1.length() >= str2.length()) {
        text = str1;
        pattern = str2;
    } else {
        text = str2;
        pattern = str1;
    }



    // string text = "abcabxabcd";
    SuffixTree st;
    st.build(pattern);

    cout << "Suffix Tree Structure:\n";
    st.print();

    return 0;
}