/*
выводстандартный вывод
Вам даны два прямоугольника, стороны которых параллельны осям координат. Определите площадь их пересечения.

Входные данные
В первой строке даны четыре числа x1
, y1
, x2
, y2
 — координаты двух противоположных углов первого прямоугольника, во второй — x3
, y3
, x4
, y4
 — координаты противоположных углов второго прямоугольника.

(−100⩽Xi,Yi⩽100)
.

Выходные данные
Выведите одно число — площадь пересечения прямоугольников.
*/

#include <iostream>
#include <cmath>

using namespace std;

int main() {
    int x1, y1, x2, y2, x3, y3, x4, y4;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

    int x_side, y_side;

    // x1 < x2
    if (x1 > x2){
        int tmp_x = x1;
        x1 = x2;
        x2 = tmp_x;
    }
    // x3 < x4
    if (x3 > x4){
        int tmp_x = x3;
        x3 = x4;
        x4 = tmp_x;
    }


    // y1 < y2
    if (y1 > y2){
        int tmp_y = y1;
        y1 = y2;
        y2 = tmp_y;
    }
    // y3 < y4
    if (y3 > y4){
        int tmp_y = y3;
        y3 = y4;
        y4 = tmp_y;
    }

    if (x2 <= x3){
        x_side = 0;
    }
    else if(x3 <= x2 && x2 <= x4){
        x_side = x2 - max(x1,x3);
    }
    else if(x4 < x2){
        x_side = x4 - max(x1, x3);
    }
    if (y4 <= y1){
        y_side = 0;
    }
    else if(y3 <= y2 && y2 <= y4){
        y_side = y2 - max(y1,y3);
    }
    else if(y4 < y2){
        y_side = y4 - max(y1, y3);
    }

    // Debag 
    // cout << "x1: " << x1 << " y1: " << y1 << endl;
    // cout << "x2: " << x2 << " y2: " << y2 << endl;
    // cout << "x3: " << x3 << " y3: " << y3 << endl;
    // cout << "x4: " << x4 << " y4: " << y4 << endl;


    if (x_side < 0 or y_side < 0) cout << 0 << endl;
    else cout << x_side * y_side << endl;
}