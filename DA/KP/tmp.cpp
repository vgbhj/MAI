#include <bits/stdc++.h>
using namespace std;

static const int INF = INT_MAX;

// Направления движения (вверх, вправо, вниз, влево)
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

// Манхэттенская эвристика
inline int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int aStar0or2BFS(const vector<string> &grid, 
                 int sx, int sy, int ex, int ey) 
{
    int n = (int)grid.size();
    int m = (int)grid[0].size();
    
    vector<vector<int>> f(n, vector<int>(m, INF));

    int startH = heuristic(sx, sy, ex, ey);
    f[sx][sy] = startH;

    // Дек, в котором будем хранить (x, y). Будем ориентироваться на f[x][y].
    deque<pair<int,int>> dq;
    dq.push_front({sx, sy});

    while (!dq.empty()) {
        auto [cx, cy] = dq.front();
        dq.pop_front();

        int currentF = f[cx][cy];
        int currentH = heuristic(cx, cy, ex, ey);
        int currentG = currentF - currentH;

        // Если мы в целевой клетке, значит получили кратчайший путь
        if (cx == ex && cy == ey) {
            return currentG;  // Минимальный g
        }

        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) {
                continue;
            }
            if (grid[nx][ny] == '#') {
                continue;
            }
            // Считаем потенциальное новое f'
            int neighH = heuristic(nx, ny, ex, ey);
            int neighG = currentG + 1; // каждый ход стоит 1
            int neighF = neighG + neighH;

            // Проверим, можем ли улучшить f[nx][ny]
            if (neighF < f[nx][ny]) {
                f[nx][ny] = neighF;  // улучшили

                // Определим, на сколько изменилось f
                int deltaF = neighF - currentF; // должно быть или 0, или 2
                if (deltaF == 0) {
                    // Добавляем в начало дека
                    dq.push_front({nx, ny});
                } else {
                    // deltaF == 2
                    dq.push_back({nx, ny});
                }
            }
        }
    }

    // Если добрались сюда, пути нет
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    int q;
    cin >> q;
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        --x1; --y1; --x2; --y2;
        int dist = aStar0or2BFS(grid, x1, y1, x2, y2);
        cout << dist << "\n";
    }

    return 0;
}
