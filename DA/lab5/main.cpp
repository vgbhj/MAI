#include <bits/stdc++.h>

using namespace std;

struct ST {
    int end;
    string s;
    int pos = 0, remainder = 0;
    // debtor - должник
    int deb_l, deb_r;

    struct node {
        int l, r;
        map<char, node*> next;
        node* par;
        node* link;

        node(int l = 0, int r = 0, int par = -1)
            : l(l), r(r), par(nullptr), link(nullptr) {}

        int get (char c) {
            if (!next.count(c))  return -1;
            return 1;
        }
    };

    node* active_node;
    node* root;
    node* from_node = nullptr;

    ST() : end(0), s(""), active_node(new node()), root(new node()) {
        // Инициализация cur_node и root как новых объектов node
    }

    ~ST() {
        delete active_node;
        delete root;
    }

    void add_node(node *par, node *child, char c){
        child->par = par;
        par->next[c] = child;
    }

    void go(){
        while(deb_l < deb_r){
            if (active_node->l + (deb_r - deb_l) <= active_node->r){
                pos = (deb_r - deb_l);
                deb_l = active_node->l + pos;
                return;
            }
            else{
                if ( active_node->next[s[deb_l]] != nullptr){
                    active_node = active_node->next[s[deb_l]];
                    deb_l = active_node->l;}
                else{
                    // если нету создать??? или не может не быть?
                    return;
                }
            }
            return;
        }
    }
    
    void split(char c){
        active_node->r = active_node->l + pos;
        // сохрраняем старую ветку
        node* tmp_node1 = new node(active_node->l + pos, end-1); 
        add_node(active_node, tmp_node1, s[active_node->l + pos]);
        // докидываем новую с новым символом
        node* tmp_node2 = new node(end-1, end-1); 
        add_node(active_node, tmp_node2, c);
        remainder = 0;
        deb_l = active_node->l + 1;
        deb_r = active_node->r;
        // добавляем линку
        if (from_node != nullptr){
            from_node->link = tmp_node2->par;
        }
        from_node = tmp_node2->par;
        active_node = active_node->par;
        // у предыдущей вершины есть линка 
        if(active_node->link != nullptr){ active_node = active_node->link;}
        // у предыдущей вершины нет линки (корень?)

        // ищем первую букву у паттерна должников
        if ( active_node->next[s[deb_l]] != nullptr){
            // cout << s[deb_l] << '\n';
            active_node = active_node->next[s[deb_l]];
            deb_l = active_node->l;
        }
        else{
            // если нету создать??? или не может не быть?
            return;
        }
    
        go();
        return;
        split(s[active_node->l + (deb_r - deb_l)]);
    }

    void add(char c) {
        // cout << c << "~!~\n";
        s += c;
        end++;
        if(remainder){
            if( s[active_node->l+pos] == c){
                pos++;
                // remainder++
            }
            else{
                // cout << pos << '\n';
                split(c);
            }

        }
        else{
            if(root->get(c) == 1){
                remainder++;
                pos++;
                active_node = root->next[c];
            }
            else{
                node* tmp_node = new node(end-1, end-1); // Устанавливаем l и r
                add_node(root, tmp_node, c);
            }
        }


    }

    // Функция для печати дерева
    void print(node* n, int depth = 0) {
        if (n == nullptr) return; // Проверка на nullptr

        // Определяем, является ли узел листом
        bool is_leaf = n->next.empty();

        // Формируем строку для вывода
        string info;
        string substring;
        if (is_leaf) {
            info = "l=" + to_string(n->l) + ", end=" + to_string(end);
            substring = s.substr(n->l, end - n->l); // Срез от l до end
        } else {
            info = "l=" + to_string(n->l) + ", r=" + to_string(n->r);
            substring = s.substr(n->l, n->r - n->l); // Срез от l до r
        }

        // Вывод информации о текущем узле
        cout << string(depth * 2, ' ') << "Node: " << info << ", \""<< substring
            << "\", link=" << n->link << endl;

        // Рекурсивный вызов для всех дочерних узлов
        for (auto& child : n->next) {
            print(child.second, depth + 1); // Используем child.second для получения указателя на узел
        }
    }


    // Вспомогательная функция для начала печати с корня
    void print() {
        cout << "Tree structure:" << endl;
        print(root); // Начинаем с корня
    }
};

int main() {
    // ABCDYFABC$
    ST st;
    string text = "ABCDYFABC$";
    for (auto _ : text) {
        st.add(_);
    }

    // Печать дерева
    st.print();

    return 0;
}
