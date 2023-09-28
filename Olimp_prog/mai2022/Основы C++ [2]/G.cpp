#include <iostream>
#include <bits/stdc++.h>

using namespace std;

long long int factorial(int n){
    if(n == 1 || n == 0){
        return 1;
    }
    return n*factorial(n-1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    // 1 2 3 4 5 6 7 8 9 0

    string n;
    cin >> n;
    int s=0;
    for(char elem: n){
        if (elem == '6' || elem == '9' || elem == '0' || elem == '4') s++;
        else if(elem == '8') s+=2;
    }

    cout << s;
}