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

    vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(m + 1, vector<int>(n + 1, -1)));
    dp[0][0][0] = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            for (int k = 0; k <= n; k++)
            {
                if (dp[i][j][k] == -1)
                    continue;
                dp[i + 1][j][k] = max(dp[i + 1][j][k], dp[i][j][k]);
                if (j + w[i] <= m && k + 1 <= n)
                {
                    dp[i + 1][j + w[i]][k + 1] = max(dp[i + 1][j + w[i]][k + 1], dp[i][j][k] + c[i]);
                }
            }
        }
    }

    int max_value = 0, best_weight = 0, best_count = 0;
    for (int j = 0; j <= m; j++)
    {
        for (int k = 0; k <= n; k++)
        {
            if (dp[n][j][k] != -1)
            {
                int value = dp[n][j][k] * k;
                if (value > max_value)
                {
                    max_value = value;
                    best_weight = j;
                    best_count = k;
                }
            }
        }
    }

    cout << max_value << endl;
    vector<int> result;
    int j = best_weight, k = best_count;
    for (int i = n - 1; i >= 0; i--)
    {
        if (k > 0 && j >= w[i] && dp[i][j - w[i]][k - 1] + c[i] == dp[i + 1][j][k])
        {
            result.push_back(i + 1);
            j -= w[i];
            k--;
        }
    }

    reverse(result.begin(), result.end());
    for (int x : result)
    {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}