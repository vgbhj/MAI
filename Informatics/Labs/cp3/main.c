#include <stdio.h>
#include <math.h>

int main(){
    double ans, f;
    double eps = 1;
    while (1 + eps/2 > 1) eps /= 2;
    printf("Машинное эпсилон для типа double = %.16e\n", eps);
    int n, cnt;
    int k = 50;
    printf("Введите число разбиений отрезка: ");
    scanf("%d", &n);
    double a = 0.1;
    double b = 0.6;
    double step = (b - a)/n;
    printf("Таблица значений ряда Тейлора и стандартной функции для f(x) = (1+x^2)/2*arctg(x)-x/2\n");
    printf("================================================================== \n");
    printf("|   x  |         sum         |      f(x)          |число итераций|\n");
    printf("================================================================== \n");
    double x = 0;
    for(int i=0; i<=n; ++i){
        double d = 1;
        x += step;
        ans = 0; // част сумма ряда
        cnt = 1; // число иттераций
        f = (1+x*x)/2*atan(x)-x/2;
        
        while (cnt < 50) {
            d = pow((-1), cnt - 1) * pow(x,2*cnt+1)/(4*cnt*cnt-1);
            // printf("%d ", d);
            ans += d;
            cnt++;
        }
        
        printf("| %.2f | %.16f | %.16f |      %d       |\n", x, ans, f, cnt);
    printf("=================================================================== \n");
    }
    
    return 0;
}