#include <bits/stdc++.h>

using namespace std;

int main(){

    int n = 5*10e5;
    vector<int> prime (n+1, 1);
    prime[0] = prime[1] = 0;
    for (int i=2; i<=n; ++i)
	    if (prime[i])
		    if (i * 1ll * i <= n)
			    for (int j=i*i; j<=n; j+=i)
				    prime[j] = 0;

    cout << prime[13];
}