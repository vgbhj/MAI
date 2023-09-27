#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        vector<int> tmp(n);
        set<int> st;
        for(int j = 0; j < n; ++j){
            cin >> tmp[j];
        }

        for(int j = 0; j < n; ++j){
            int sm = tmp[j];
            cout << sm << ' ' << j << ' ' << j << ' ' << '\n';
            st.insert(sm);
            for(int k = j+1; k < n; ++k){
                sm = sm ^ tmp[k];
                cout << sm << ' ' << j << ' ' << k << ' ' << '\n';
                st.insert(sm);
            }
        }
        cout << st.size() << '\n';
    }
}