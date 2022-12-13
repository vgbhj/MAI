#include <stdio.h>
#include <math.h>

//Задаём машинный эпсилон

double get_eps()
{
    double eps = 1;
    while (1.0f + eps / 2.0f > 1.0f)
    {
        eps /= 2.0f;
    }
    return eps;
}

//Метод дихотомии

double dichotomy_method(double a, double b, double (*fn)(double))
{
    double eps = get_eps();
    while (b - a > eps * 10e3)
    {
        double c = (a + b) / 2;
        if (fn(a) * fn(c) > 0)
            a = c;
        else
            b = c;
    }
    return (a + b) / 2;
}

//Функция

double fn(double x)
{
    return cos(2/x) - 2*sin(1/x) + 1/x;
}


int main()
{
    double x = dichotomy_method(1, 2, fn);
    printf("Dichotomy_method: %.4f\n", x);
}