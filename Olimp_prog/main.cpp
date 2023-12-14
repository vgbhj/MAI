#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using ll = long long;

int main() {
    // Чтение входных данных
    ll n, X;
    cin >> n >> X;

    vector<pair<ll, ll>> trees; // Вектор для хранения координат деревьев

    for (ll i = 0; i < n; ++i) {
        ll x, y;
        cin >> x >> y;
        trees.push_back({x, y});
    }

    // Сортировка деревьев по координатам x
    sort(trees.begin(), trees.end());

    long long totalDistance = 0; // Сумма расстояний от точки до всех деревьев
    for (ll i = 0; i < n; ++i) {
        totalDistance += abs(trees[i].first - (i + 1) * 1LL * X);
    }

    // Вывод результатов
    cout << totalDistance << " " << (n + 1) / 2 * X << endl;

    return 0;
}
