#include <bits/stdc++.h>

using namespace std;

using ll = int64_t;
using pii = pair<int, int>;

bool cmp(const pair<int, int>& a, const pair<int, int>& b){
    if(a.first == b.first){
        return a.second > b.second;
    }
    return a.first < b.first;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<pii> v;
    for(int i = 0; i < n; ++i){
        int a, b;
        cin >> a >> b;
        v.push_back({a, b});
    }

    sort(v.begin(), v.end());
    vector<int> ans;
    for(int i = 0; i < v.size(); ++i){
        int count = 0;
        for(int j = i; v[j].first <= v[i].second; ++j){
            if(v[j].second <= v[i].second){
                count++;
            }
        }
        ans.push_back(count);
    }

    sort(ans.begin(), ans.end());
    cout << ans[ans.size()-1];
}