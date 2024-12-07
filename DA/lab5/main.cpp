#include <bits/stdc++.h>
using namespace std;

struct SuffixTree {
    struct Node {
        int l, r; // Индексы на строке: [l, r)
        map<char, Node*> next; // Переходы по символам
        Node* link; // Суффиксная ссылка
        int string_mask; // Маска строк, которые присутствуют в поддереве узла
        Node(int l = 0, int r = 0) : l(l), r(r), link(nullptr), string_mask(0) {}
        int length(int end) const { return min(r, end + 1) - l; } // Длина ребра
    };

    string s;
    Node* root; // Корень дерева
    Node* active_node; // Текущая активная вершина
    int active_edge, active_length; // Активное ребро и длина
    int remainder; // Оставшиеся суффиксы
    int pos; // Позиция в строке
    int size; // Размер строки

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

    void dfs(Node* node, int depth, int& max_len, set<string>& results) {
        if (!node) return;

        // Обновляем маску узла
        if (node->l != node->r) {
            if (s[node->l] == '#') node->string_mask |= 1;
            if (s[node->l] == '$') node->string_mask |= 2;
        }

        for (auto& p : node->next) {
            dfs(p.second, depth + p.second->length(pos), max_len, results);
            node->string_mask |= p.second->string_mask; // Передаем маску вверх
        }

        // Если узел пересекается между двумя строками
        if (node->string_mask == 3) {
            int len = depth + node->length(pos);
            if (len > max_len) {
                max_len = len;
                results.clear();
            }
            if (len == max_len) {
                results.insert(s.substr(node->l - max_len, max_len));
            }
        }
    }

    pair<int, set<string>> find_longest_common_substring() {
        int max_len = 0;
        set<string> results;
        dfs(root, 0, max_len, results);
        return {max_len, results};
    }
};

int main() {
    string a, b;
    getline(cin, a);
    getline(cin, b);

    string combined = a + "#" + b + "$";
    SuffixTree st;
    st.build(combined);
    st.print();
    // auto result = st.find_longest_common_substring();
    // cout << result.first << endl;
    // for (const auto& substr : result.second) {
    //     cout << substr << endl;
    // }

    return 0;
}
