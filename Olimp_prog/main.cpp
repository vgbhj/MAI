#include <bits/stdc++.h>

using namespace std;

vector<int> z_func(const string& s){
    int n = s.size();
    vector<int> z(n);
    z[0] = n;
    int l = 0, r = 0;
    for(int i = 1; i < n; ++i){
        if(i < r){
            z[i] = min(z[i - l], r - i);
        }
        while (i + z[i] < n and s[z[i]] == s[i+z[i]])
        {
            ++z[i];
        }
        if(i + z[i] > r){
            l = i;
            r = l + z[i];
        }
    }
    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int q, t;
    cin >> q;
    map<int, int> mp;
    while (q--)
    {
        cin >> t;
        if(t == 1){
            
        }
        if(t == 2){
            
        }
        if(t == 3){
            
        }
    }
    
}