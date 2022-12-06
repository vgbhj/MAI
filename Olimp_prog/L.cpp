#include <bits/stdc++.h>

using namespace std;


string alph = "0123456789abcdefghijklmnopqrstuvwxyz";
map <char, int> alph_dict;

void fill_alph_dict(){
    for(int i=0; i<alph.length(); ++i){
        alph_dict[alph[i]] = i;
    }
}

string from10toB(long long a, long long b){
    string s = "";
    while(a){
        s += alph[a%b];
        a /= b;
    }

    reverse(s.begin(), s.end());
    if(s == ""){
        s = "0";
    }
    return s;
}

long long pw(int a, int b){
    long long res = 1;
    for(int i=0; i<b; ++i){
        res *= a;
    }
    return res;
}

long long fromAto10(string s, int from){
    if(from == 10){
        return stoll(s);
    }
    reverse(s.begin(), s.end());
    long long n=0;
    for(auto i=0; i<s.length(); ++i){
        n += (alph_dict[s[i]])*(pw(from, i));
    }
    return n;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    long long n1;
    string n2;
    vector<int> num1(15,0);
    vector<int> num2(15,0);
    vector<int> sum1(15,0);
    vector<int> sum2(15,0); 
    char dot;

    while(cin >> n1 >> dot >> n2){
        for(int i=14; i>0; --i){
            if(n1!=0){
                num1[i] = n1%10;
                n1 /= 10;
            }
            else{
                break;
            }
        }
        for(int i=0; i<n2.length(); ++i){
            num2[i] = n2[i] - '0';
        }

        for(int i=14; i>0; --i){
            sum1[i] = (sum1[i]+num1[i])%10;
            sum1[i+1]
        }
    }

    for(int i=0; i<15; ++i){
        cout << sum1[i];
    }
    cout << ".";
    for(int i=0; i<15; ++i){
        cout << sum2[i];
    }
}