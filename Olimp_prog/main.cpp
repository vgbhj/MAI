#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<int> v(n);
    for(int i = 0; i < n; ++i){
        cin >> v[i];
    }



    int mx = v[0];
    int ans = 0;
    for(int i = 1; i < n; ++i){
        mx = max(mx, v[i]);
        if(mx > v[i]){
            ans = -1;
            break;
        }
    }
    ans == -1 ? cout << "-1\n" : cout << mx - v[0] << '\n';
}