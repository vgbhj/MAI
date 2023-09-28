/*

Вася изучает позиционные системы счисления, ему задали перевести кучу чисел из одной системы счисления в другую. Так как Вася пока не очень уверен в своих умениях по переводу чисел между системами счисления, он попросил помочь вас ему с проверкой ответов.

Входные данные
В первой строке вам даны два числа A
 и B
 (2⩽A,B⩽36)
 — системы счисления из которой нужно перевести и в которую нужно перевести число соответственно.

В следующей строке дано неотрицательное число N
 — число, которое нужно перевести в другую систему счисления (цифры большие девяти обозначаются маленькими буквами латинского алфавита). Гарантируется, что в десятичной системе счисления N⩽1018
.

Выходные данные
Выведите данное число в требуемой системе счисления (цифры большие девяти обозначаются маленькими латинскими буквами).*/

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
    
    int from, to;
    string num;
    long long n;
    fill_alph_dict();
    cin >> from >> to;
    cin >> num;
    n = fromAto10(num, from);
    // cout << n << "\n";
    cout << from10toB(n, to);
}