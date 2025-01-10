#include <bits/stdc++.h>

using namespace std;

struct Node {
    int x, y;
    int g;
    int h; 

    int f() const { return g + h; }

    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Node& other) const {
        return f() > other.f();
    }
};

struct NodeHash {
    size_t operator()(const Node& node) const {
        return hash<int>()(node.x) ^ hash<int>()(node.y);
    }
};

class AStar {
public:
    AStar(const vector<vector<char>>& grid) : grid(grid), rows(grid.size()), cols(grid[0].size()) {}

    int findPath(Node start, Node goal) {
        unordered_set<Node, NodeHash> closedSet;
        priority_queue<Node> openSet;

        start.h = heuristic(start, goal);
        openSet.push(start);

        while (!openSet.empty()) {
            Node current = openSet.top();
            openSet.pop();

            if (current.x == goal.x && current.y == goal.y) {
                return current.g; 
            }

            closedSet.insert(current);

            for (Node neighbor : getNeighbors(current)) {
                if (closedSet.find(neighbor) != closedSet.end()) {
                    continue; 
                }

                neighbor.g = current.g + 1;
                neighbor.h = heuristic(neighbor, goal);

                openSet.push(neighbor);
            }
        }

        return -1;
    }

private:
    vector<vector<char>> grid;
    int rows, cols;

    int heuristic(const Node& a, const Node& b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }

    vector<Node> getNeighbors(Node& node) {
        vector<Node> neighbors;
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (auto& dir : directions) {
            int newX = node.x + dir[0];
            int newY = node.y + dir[1];

            if (isWalkable(newX, newY)) {
                neighbors.push_back({newX, newY, 0, 0});
            }
        }

        return neighbors;
    }

    bool isWalkable(int x, int y) {
        return x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == '.';
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
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
    AStar astar(grid);

    for (int i = 0; i < q; ++i) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        Node start = {x1 - 1, y1 - 1, 0, 0};
        Node goal = {x2 - 1, y2 - 1, 0, 0};

        int result = astar.findPath(start, goal);
        cout << result << endl;
    }

    return 0;
}
