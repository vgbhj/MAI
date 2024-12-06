#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int minDays(vector<int>& bloomDay, int m, int k) {
        if(bloomDay.size() < m*k){
            return -1;
        }
        vector<long long> pref(bloomDay.size()-k+1);
        // Form 1st pref sum
        pref[0] = accumulate(bloomDay.begin(), bloomDay.begin()+k, 0);

        // Debug
        for(auto _ : pref){
            cout << _ << ' ';
        }
        cout << '\n';

        for(int i = 1; i < bloomDay.size()-k+1; ++i){
            pref[i] = pref[i-1]+bloomDay[k+i-1]-bloomDay[i-1];
        }

        // Debug
        for(auto _ : pref){
            cout << _ << ' ';
        }
        cout << '\n';
        return 1;
    }
};

int main(){
    Solution solv;
    vector<int> v = {7,7,7,7,12,7,7};
    cout << solv.minDays(v, 2, 3);
    return 0;
}

/*

найти m минимальных НЕПЕРЕСЕКАЮЩИЕСЯ подотрезка длинны k

*/