#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int n;
    cin >> n;
    vector<int> arr(n);
    vector<int> count(4);
	count[0] = 0;
    int swap = 0;
    for(int i = 0; i < n; ++i){
        cin >> arr[i];
        count[arr[i]]++;
    }

    count[3] = count[2] + count[1];
	count[2] = count[1] + count[0];
	count[1] = count[0];

    for(int i = 0; i < n; ++i){
        if(i < count[2]){
            if(arr[i] == 3){
                for(int j = n - 1; j >= count[2]; --j){
                    if(arr[j] == 1){
                        int tmp_a = arr[i];
                        arr[i] = arr[j];
                        arr[j] = tmp_a;
                        swap++;
                        break;
                    }
                }
            }
            else if (arr[i] == 2){
                for(int j = count[2]; j < n; ++j){
                    if(arr[j] == 1){
                        int tmp_a = arr[i];
                        arr[i] = arr[j];
                        arr[j] = tmp_a;
                        swap++;
                        break;
                    }
                }
            }
        }
        // 2
        else if(i < count[3]){
            if(arr[i] != 2){
                swap++;
                for(int j = count[3]; j < n; ++j){
                    if(arr[j] == 2){
                        int tmp_a = arr[i];
                        arr[i] = arr[j];
                        arr[j] = tmp_a;
                        break;
                    }
                }
            }
        }

        // for(auto _ : arr){
        //     cout << _ << ' ';
        // }
        // cout << '\n';
    }

    cout << swap;
    return 0;
}