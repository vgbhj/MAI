#include <bits/stdc++.h>

using namespace std;

vector<int> computeZ(const string& s) {
    int n = s.size();
    vector<int> Z(n);
    int L = 0, R = 0;
    for (int i = 1; i < n; ++i) {
        if (i <= R) {
            Z[i] = min(R-i+1, Z[i-L]);
        }
        while (i+Z[i] < n && s[Z[i]] == s[i+Z[i]]) {
            ++Z[i];
        }
        if (i+Z[i]-1 > R) {
            L = i;
            R = i+Z[i]-1;
        }
    }
    return Z;
}

int main() {

    cout << string::npos;

    string text, pattren;
    getline(cin, text);
    getline(cin, pattren);

    string combined = pattren + '$' + text;
    vector<int> z = computeZ(combined);

    int patterntLen = pattren.size();
    int combinedLen = combined.size();

    for(int i = patterntLen + 1; i < combinedLen; ++i){
        if(z[i] == patterntLen){
            cout << (i - patterntLen - 1) << '\n';
        }
    } 

    return 0;
}
