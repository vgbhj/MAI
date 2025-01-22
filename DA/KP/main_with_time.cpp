#include <bits/stdc++.h>
#include <chrono>

using namespace std;

using pii = pair<int, int>;

struct Node {
    int f, x, y;

    Node(int f, int x, int y) : f(f), x(x), y(y) {}

    bool operator<(const Node& other) const {
        return f > other.f;
    }
};

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


int a_star(const vector<vector<char>>& grid, pii start, pii goal, int n, int m) {
    deque<Node> open_set;
    vector<vector<int>> f_score(n, vector<int>(m, INT_MAX));
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    f_score[start.first][start.second] = heuristic(start.first, start.second, goal.first, goal.second);
    open_set.push_front(Node(heuristic(start.first, start.second, goal.first, goal.second), start.first, start.second));

    vector<pii> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!open_set.empty()) {
        Node current = open_set.front();
        open_set.pop_front();

        int x = current.x, y = current.y;
        int h = heuristic(current.x, current.y, goal.first, goal.second);
        int g = f_score[x][y] - h;

        if (x == goal.first && y == goal.second) {
            return g;
        }

        if (visited[x][y]) continue;
        visited[x][y] = true;

        for (auto [dx, dy] : directions) {
            int new_x = x + dx, new_y = y + dy;

            if (new_x >= 0 && new_x < n && new_y >= 0 && new_y < m && 
                grid[new_x][new_y] == '.' && !visited[new_x][new_y]) {
                
                int new_h = heuristic(new_x, new_y, goal.first, goal.second);
                int new_g = g + 1;
                int new_f = new_h + new_g;

                if (new_f < f_score[new_x][new_y]) {
                    f_score[new_x][new_y] = new_f;
                    if ((new_f - current.f) == 0){
                        open_set.push_front(Node(new_f, new_x, new_y));
                    } else {
                        open_set.push_back(Node(new_f, new_x, new_y));
                    }
                }
            }
        }
    }

    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    int n, m;
    cin >> n >> m;

    vector<vector<char>> grid(n, vector<char>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
        }
    }

    int q;
    cin >> q;

    long long total_time = 0; 
    int query_count = 0;

    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        pii start = {x1 - 1, y1 - 1};
        pii goal = {x2 - 1, y2 - 1};
        if (start.first < 0 || start.second < 0 || 
            goal.first < 0 || goal.second < 0 || 
            start.first >= n || start.second >= m || 
            goal.first >= n || goal.second >= m || 
            grid[start.first][start.second] == '#' || 
            grid[goal.first][goal.second] == '#') {
            cout << -1 << "\n";
            continue;
        }

        auto start_time = chrono::high_resolution_clock::now();

        int result = a_star(grid, start, goal, n, m);

        auto end_time = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        cerr << "Время выполнения для запроса: " << elapsed.count() << " миллисекунд.\n";

        total_time += elapsed.count();
        query_count++;

        cout << result << "\n";
    }

    if (query_count > 0) {
        double average_time = static_cast<double>(total_time) / query_count;
        cout << "Среднее время выполнения по всем запросам: " << average_time << " миллисекунд.\n";
    }

    return 0;
}