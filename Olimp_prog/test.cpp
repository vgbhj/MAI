#include <bits/stdc++.h>

using namespace std; 

const int64_t MOD = 1000000000+7; 

int64_t fact(int n){
    int64_t factorial = 1;
    for(int i = 1; i <= n; ++i) {
            factorial = (factorial*i)%MOD;
        }
    return factorial;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    string s;
    cin >> s;
    int o=0, c=0; 
    for(auto elem : s){
        if(elem == '('){
            o++;
        }
        else{
            c++;
        }
    }
    int mx = max(o,c);
    int mn = min(o,c);
    //Debug
    // cout << mn << " " << mx << "\n";
    int64_t ans = 0;
    for(int i=1; i<=mn; ++i){
        ans += ((fact(mn)/(fact(mn-i)*fact(i))) * (fact(mx)/(fact(mx-i)*fact(i))))%MOD;
        //Debug
        // cout << fact(mn) << " " << fact(mn-i) << " " <<  fact(i) << "\n"; 
        // cout << fact(mn)/(fact(mn-i)*fact(i)) << "\n";
        // cout << fact(mx)/(fact(mx-i)*fact(i)) << "\n";
        // cout << ans << "\n";
        // cout << "=====\n";
    }

    cout << ans%MOD;
}