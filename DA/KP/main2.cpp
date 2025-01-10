#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

struct Node {
    int f, x, y;
    Node(int f, int x, int y) : f(f), x(x), y(y) {}
    bool operator<(const Node& other) const {
        return f > other.f; // Минимальный f имеет приоритет
    }
};

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int a_star(const vector<vector<char>>& grid, pii start, pii goal, int n, int m) {
    priority_queue<Node> open_set;
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<int>> g_score(n, vector<int>(m, INT_MAX));

    g_score[start.first][start.second] = 0;
    open_set.emplace(heuristic(start.first, start.second, goal.first, goal.second), start.first, start.second);

    vector<pii> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!open_set.empty()) {
        Node current = open_set.top();
        open_set.pop();

        int x = current.x, y = current.y;

        if (visited[x][y]) continue; // Уже посещен
        visited[x][y] = true;

        if (x == goal.first && y == goal.second) {
            return g_score[x][y];
        }

        for (auto [dx, dy] : directions) {
            int new_x = x + dx, new_y = y + dy;
            if (new_x >= 0 && new_x < n && new_y >= 0 && new_y < m && grid[new_x][new_y] == '.') {
                int tentative_g = g_score[x][y] + 1;

                if (tentative_g < g_score[new_x][new_y]) {
                    g_score[new_x][new_y] = tentative_g;
                    int f = tentative_g + heuristic(new_x, new_y, goal.first, goal.second);
                    open_set.emplace(f, new_x, new_y);
                }
            }
        }
    }

    return -1; // Цель недостижима
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

    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        pii start = {x1 - 1, y1 - 1};
        pii goal = {x2 - 1, y2 - 1};

        cout << a_star(grid, start, goal, n, m) << "\n";
    }

    return 0;
}