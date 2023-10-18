#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int q, r;
    set<int> st;
    cin >> q >> r;
    for(int a = 1; a <= q; ++a){
        for(int d = 1; d <= r; ++d){
            int sm = a;
            int count = 0;
            while (sm <= q*r*r*r)
            {
                if(sm == q || sm == q*r || sm == q*r*r) {cout << sm << " sm\n";count++;}
                sm += d;
            }
         
        if(count > 2){
            cout << a << ' ' << d << '\n';
            st.insert(d);
        }   
        }
    }
    cout << st.size();
}