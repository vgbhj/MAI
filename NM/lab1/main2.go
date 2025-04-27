package main

import (
	"fmt"
	"log"
)

// a[2..n] — нижняя диагональ (a[1] не используется, т.к. в первом уравнении нет x0),
// b[1..n] — главная диагональ,
// c[1..n-1] — верхняя диагональ (c[n] не используется),
// d[1..n] — правая часть.
func solve_tridiagonal(a, b, c, d []float64) []float64 {
	n := len(b) - 1
	// cPrime и dPrime — вспомогательные векторы длины n+1 (индексация с единицы)
	cPrime := make([]float64, n+1)
	dPrime := make([]float64, n+1)

	// Шаг 1: инициализация
	if b[1] == 0 {
		log.Fatal("Деление на ноль на первом шаге прогонки")
	}
	cPrime[1] = c[1] / b[1]
	dPrime[1] = d[1] / b[1]

	// Шаг 2: прямой ход
	for i := 2; i <= n; i++ {
		znam := b[i] - a[i]*cPrime[i-1]
		if i < n {
			cPrime[i] = c[i] / znam
		}
		dPrime[i] = (d[i] - a[i]*dPrime[i-1]) / znam
	}

	// Шаг 3: обратный ход
	x := make([]float64, n+1)
	x[n] = dPrime[n]
	for i := n - 1; i >= 1; i-- {
		x[i] = dPrime[i] - cPrime[i]*x[i+1]
	}

	return x
}

func main() {
	n := 5
	a := make([]float64, n+1) // нижняя диагональ
	b := make([]float64, n+1) // главная диагональ
	c := make([]float64, n+1) // верхняя диагональ
	d := make([]float64, n+1) // правая часть

	b[1], c[1], d[1] = 18, -9, -81
	a[2], b[2], c[2], d[2] = 2, -9, 4, 71
	a[3], b[3], c[3], d[3] = -9, 21, -8, -39
	a[4], b[4], c[4], d[4] = -4, -10, 5, 64
	a[5], b[5], d[5] = 7, 12, 3

	x := solve_tridiagonal(a, b, c, d)

	fmt.Println("Решение:")
	for i := 1; i <= n; i++ {
		fmt.Printf("x%d = %.6f\n", i, x[i])
	}

	// Проверка: A * x ≈ d
	fmt.Println("\nПроверка (A * x):")
	for i := 1; i <= n; i++ {
		Ax := b[i] * x[i]
		if i > 1 {
			Ax += a[i] * x[i-1]
		}
		if i < n {
			Ax += c[i] * x[i+1]
		}
		fmt.Printf("строка %d: %.6f ≈ %.6f\n", i, Ax, d[i])
	}
}
