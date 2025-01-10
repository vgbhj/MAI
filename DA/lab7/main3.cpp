#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n, m;
    cin >> n >> m;

    vector<int> w(n), c(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> c[i];
    }

    vector<long long> dp(m + 1, 0);
    vector<int> count(m + 1, 0);

    for (int i = 0; i < n; i++) {
        for (int j = m; j >= w[i]; j--) {
            if (dp[j - w[i]] + c[i] > dp[j]) {
                dp[j] = dp[j - w[i]] + c[i];
                count[j] = count[j - w[i]] + 1; 
            }
        }
    }

    long long max_value = 0;
    int best_weight = 0;
    for (int j = 0; j <= m; j++) {
        if (dp[j] > max_value) {
            max_value = dp[j];
            best_weight = j;
        }
    }

    cout << max_value << endl;

    vector<int> result;
    int j = best_weight;
    for (int i = n - 1; i >= 0; i--) {
        if (j >= w[i] && dp[j] == dp[j - w[i]] + c[i]) {
            result.push_back(i + 1);
            j -= w[i];
        }
    }

    reverse(result.begin(), result.end());
    for (int x : result) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
