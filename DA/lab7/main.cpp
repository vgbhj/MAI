#include <bits/stdc++.h>

using namespace std;

using pii = pair<int,int>;

void findAns(int k, int s, vector<int>& ans, vector<vector<pii>>& dp, vector<int>& w){
    if (dp[k][s].first == 0)
        return;
    
    if (dp[k - 1][s].first == dp[k][s].first){
        findAns(k - 1, s, ans, dp, w);
    }
    else{
        findAns(k - 1, s - w[k - 1], ans, dp, w);
        ans.push_back(k);
    }

}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n, w;
    cin >> n >> w;
    vector<int> m(n);
    vector<int> c(n);
    for(int i = 0; i < n; ++i){
        cin >> m[i];
        cin >> c[i];
    }

    vector<vector<pii>> dp(n+1, vector<pii>(w+1));
    for(int i = 1; i <= n; ++i){
        for(int j = 1; j <= w; ++j){
            dp[i][j] = dp[i - 1][j];
            if (j - m [i - 1] >= 0){
                int tmp_k = dp[i - 1][j - m[i-1]].second + 1;
                if ((dp[i - 1][j - m[i-1]].first + c[i-1] ) * tmp_k > dp[i][j].first * dp[i][j].second){
                    dp[i][j].first = (dp[i - 1][j - m[i-1]].first + c[i-1] ) * tmp_k ;
                    dp[i][j].second = tmp_k;
                }
            }
        }
    }
    vector<int> ans;
    findAns(n, w, ans, dp, m);
    cout << dp[n][w].first << '\n';
    for(auto _ : ans){
        cout << _ << ' ';
    }
    return 0;
}