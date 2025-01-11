#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

class Graph {
public:
    int V;
    vector<vector<ll>> capacity;

    Graph(int V) {
        this->V = V;
        capacity.resize(V, vector<ll>(V, 0));
    }

    void addEdge(int u, int v, ll cap) {
        capacity[u][v] += cap; 
    }

    bool bfs(int s, int t, vector<int>& parent) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < V; v++) {
                if (!visited[v] && capacity[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                    if (v == t) return true;
                }
            }
        }

        return false;
    }

    ll edmondsKarp(int source, int sink) {
        ll maxFlow = 0;
        vector<int> parent(V);

        while (bfs(source, sink, parent)) {
            ll pathFlow = INF;

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, capacity[u][v]);
            }

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                capacity[u][v] -= pathFlow;
                capacity[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        return maxFlow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    Graph g(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        ll c;
        cin >> u >> v >> c;
        --u, --v;
        g.addEdge(u, v, c);
    }

    cout << g.edmondsKarp(0, n - 1) << endl;

    return 0;
}
