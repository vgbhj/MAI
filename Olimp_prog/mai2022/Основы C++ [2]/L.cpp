/*
Задание очень простое, вам нужно сложить все данные числа.

Входные данные
В каждой строке входного файла вам дано одно число — xi
 (0⩽xi⩽1015)
. Каждое из чисел xi
 дано с точностью ровно 15
 знаков после запятой. Количество строк не превышает 100
.

Выходные данные
Выведите результат сложения с точностью ровно 15
 знаков после запятой, лидирующие нули у чисел выводить запрещено.*/

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
    vector<int> num2(15,0);
    long long sum1=0;
    vector<int> sum2(15,0); 
    char dot;

    // for(int i=14; i>=0; --i){
    //     cout << sum2[i];
    // }

    // cout << "\n";

    while(cin >> n1 >> dot >> n2){
        for(int i=0; i<n2.length(); ++i){
            num2[i] = (int)n2[i] - 48;
            // cout << num2[i] << " " << n2[i] << "\n";
        }

        for(int i=14; i>=0; --i){
            // cout << sum2[i] << "\n";
            // cout << (sum2[i]+num2[i]) << "\n";
            // cout << sum2[i] <<" " << (sum2[i]+num2[i])%10 << " " << i << "\n";
            if(i!=0){
                sum2[i-1] += (sum2[i]+num2[i])/10;
            }
            else{
                sum1 += (sum2[i]+num2[i])/10;
            }
            sum2[i] = (sum2[i]+num2[i])%10;
        }

        sum1 += n1;
    }

    cout << sum1;
    cout << ".";
    for(int i=0; i<15; ++i){
        cout << sum2[i];
    }
}