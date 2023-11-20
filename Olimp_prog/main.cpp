#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using pii = pair<ll, ll>;

bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(n); ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
 
bool isPerfectSquare(int n) {
    int root = sqrt(n);
    return root * root == n;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    int tmp_n = 0;
    int ans = 0;
    for(int i = 0; i < n; ++i){
        char k;
        cin >> k;
        if(isPrime(k - '0')) {
            ans++; tmp_n = 0;
            }
        else{
            tmp_n *= 10;
            tmp_n += (k - '0');            
        }
        // cout << tmp_n << ;
        if(tmp_n){
            if(isPerfectSquare(tmp_n) || isPrime(tmp_n)) {
            ans++; tmp_n = 0;
            }
        }
    }

    cout << ans;
}