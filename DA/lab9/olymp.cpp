#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct network
{
    struct edge {
        int from, to;
        ll f, c;
    };

    int n;
    vector<edge> e;
    vector<vector<int>> g;
    vector<int> d, ptr;

    network(int _n) : n(_n), g(n), d(n), ptr(n) {};

    void add_edge(int u, int v, ll c) {
        g[u].push_back(e.size());
        e.push_back({u, v, 0, c});
        g[v].push_back(e.size());
        e.push_back({v, u, 0, 0});
    }

    bool bfs(int s, int t, ll b){
        d.assign(n, -1);
        d[s] = 0;
        queue<int> q;
        q.push(s);
        while (q.size()){
            int u = q.front();
            q.pop();
            for (int id : g[u]){
                int v = e[id].to;
                if (e[id].c - e[id].f >= b && d[e[id].to] == -1){
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
        return d[t] != -1;
    }
    
    ll dfs(int u, int t, ll b, ll flow){
        if (u == t){
            return flow;
        }
        for(; ptr[u] < g[u].size(); ++ptr[u]){
            int id = g[u][ptr[u]];
            int v = e[id].to;
            if(d[v] == d[u] + 1 && e[id].c - e[id].f >= b){
                ll can_push = dfs(v,t,b, min(flow, e[id].c - e[id].f));
                if(can_push > 0){
                    e[id].f += can_push;
                    e[id ^ 1].f -= can_push;
                    return can_push;
                }
                
            }
        }
        return 0;
    }

    ll dinic(int s, int t, ll b){
        ll flow = 0;
        while(bfs(s,t,b)){
            ptr.assign(n,0);
            while(1){
                ll pushed = dfs(s,t,b, INF);
                if (pushed){
                    flow += pushed;
                } else{
                    break;
                }
            }
        }
        return flow;
    }

    const ll INF = 1e18;
    const ll MAX_B = 1 << 10;

    ll max_flow(int s, int t){
        ll ans = 0;
        for (ll b = MAX_B; b > 0; b /= 2){
            ans += dinic(s, t, b);
        }
        return ans;
    }

};


int main(){
    cin.tie(0), cout.tie(0);
    int n, m;
    cin >> n >> m;
    network f(n);
    for(int i = 0; i < m; ++i){
        int u, v;
        ll w;
        cin >> u >> v >> w;
        f.add_edge(--u, --v, w);
    }
    ll flow = f.max_flow(0, n - 1);
    cout << flow;
    return 0;
}