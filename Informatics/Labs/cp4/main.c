#include <stdio.h>
#include <math.h>

double get_eps()
{
    double eps = 1;
    while (1.0f + eps / 2.0f > 1.0f)
    {
        eps /= 2.0f;
    }
    return eps;
}

double dichotomy_method(double a, double b, double (*fn)(double))
{
    double eps = get_eps();
    while (fabs(a - b) > eps)
    {
        double c = (a + b) / 2;
        if (fn(a) * fn(c) > 0)
            a = c;
        else
            b = c;
    }
    return (a + b) / 2;
}


double fn(double x)
{
    return x + sqrt(x) + pow(x, 1.0/3.0) - 2.5;
}


int main()
{
    double x = dichotomy_method(0.4, 1, fn);
    printf("Dichotomy_method: %.4f\n", x);
}