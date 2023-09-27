#include <bits/stdc++.h>

using namespace std;

class segtree{
    int n;
    vector<int> data, delay;

    segtree(const vector<int>& v) : n(v.size()), data(4 * n), delay(4 * n){
        build(1, 1, n, v);
    }

    segtree(int _n) : n(_n), data(4 * n), delay(4 * n){

    } 

    void build(int id, int l, int r, const vector<int>& v){
        if(l == r){
            data[id] = v[l -1];
        }
        int m = (l + r) / 2;
        build(id * 2, l , m, v);
        build(id * 2 + 1, m + 1, r, v);
        data[id * 2] = max(data[id * 2], data[id * 2 +1]);
    }

    void set(int p, int x){
        set(1, 1, n, p, x);
    }

    void set(int id, int l, int r, int p, int x){
        if(l == r){
            data[id] = x;
            return;
        }
        int m = (l + r) / 2;
        if(p <= m){
            set(id * 2, l, m, p, x);
        }
        else{
            set(id * 2 + 1, m + 1, r, p, x);
        }
        data[id * 2] = max(data[id * 2], data[id * 2 + 1]);
    }

    int get(int l, int r){
        return get(1, 1, n, l, r);
    }
    int get(int id, int l, int r, int ql, int qr){
        if(ql <= l and r <= qr){
            return data[id];
        }
        int m = (l + r) / 2;
        if(r <= m){
            return get(id * 2, l, m, ql, qr);
        }
        if(l > m){
            return get(id * 2 + 1, m + 1, r, ql, qr);
        }

        return max(get(id * 2, l, m, ql, qr), get(id * 2 + 1,m + 1, r, ql, qr));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int i = 5 ^ 2;
    cout << i;
    
}