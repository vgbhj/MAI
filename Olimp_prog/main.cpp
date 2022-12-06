#include <bits/stdc++.h>

using namespace std; 

const double pi = acos(-1);
const int64_t MOD = 1e9+7; 

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

    double a, h;
    cin >> a >> h;
    cout << setprecision(6);
    cout << fixed;
    cout << 4*pi*(a*sqrt(3)/2+h)*(a*sqrt(3)/2+h);
    // cout << 4*pi*(a*a*3/4+h*a*sqrt(3)+h*h);
    // cout << 4*pi*(a/2+h)*(a/2+h);
    // cout << 4*pi*(a/2+h);
}