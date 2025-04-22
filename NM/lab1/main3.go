package main

import (
	"fmt"
	"log"
	"math"
)

// jacobi решает Ax=b методом простых итераций.
// Возвращает (x, numIter).
func jacobi(A [][]float64, b []float64, x0 []float64, tol float64, maxIter int) ([]float64, int) {
	n := len(b)
	x := make([]float64, n)
	if x0 != nil {
		copy(x, x0)
	}
	xNew := make([]float64, n)

	for k := 1; k <= maxIter; k++ {
		// для каждой компоненты
		for i := 0; i < n; i++ {
			sigma := 0.0
			for j := 0; j < n; j++ {
				if j == i {
					continue
				}
				sigma += A[i][j] * x[j]
			}
			if A[i][i] == 0 {
				log.Fatalf("Нулевой диагональный элемент A[%d][%d]", i, i)
			}
			xNew[i] = (b[i] - sigma) / A[i][i]
		}
		// проверяем сходимость в бесконечной норме
		diff := 0.0
		for i := 0; i < n; i++ {
			d := math.Abs(xNew[i] - x[i])
			if d > diff {
				diff = d
			}
		}
		copy(x, xNew)
		if diff < tol {
			return x, k
		}
	}
	return x, maxIter
}

// gaussSeidel решает Ax=b методом Зейделя.
// Возвращает (x, numIter).
func gaussSeidel(A [][]float64, b []float64, x0 []float64, tol float64, maxIter int) ([]float64, int) {
	n := len(b)
	x := make([]float64, n)
	if x0 != nil {
		copy(x, x0)
	}

	for k := 1; k <= maxIter; k++ {
		xOld := make([]float64, n)
		copy(xOld, x)

		for i := 0; i < n; i++ {
			sigma := 0.0
			// используем уже обновлённые x[0..i-1] и старые xOld[i+1..n-1]
			for j := 0; j < i; j++ {
				sigma += A[i][j] * x[j]
			}
			for j := i + 1; j < n; j++ {
				sigma += A[i][j] * xOld[j]
			}
			if A[i][i] == 0 {
				log.Fatalf("Нулевой диагональный элемент A[%d][%d]", i, i)
			}
			x[i] = (b[i] - sigma) / A[i][i]
		}

		// проверяем сходимость
		diff := 0.0
		for i := 0; i < n; i++ {
			d := math.Abs(x[i] - xOld[i])
			if d > diff {
				diff = d
			}
		}
		if diff < tol {
			return x, k
		}
	}
	return x, maxIter
}

func main() {
	A := [][]float64{
		{21, -6, -9, -4},
		{-6, 20, -4, 2},
		{-2, -7, 20, 3},
		{4, 9, 6, 24},
	}
	b := []float64{127, -144, -236, 5}

	tol := 1e-6
	maxIter := 10000

	// начальное приближение нулевое
	xJ, itJ := jacobi(A, b, nil, tol, maxIter)
	xS, itS := gaussSeidel(A, b, nil, tol, maxIter)

	fmt.Printf("Точность eps = %.1e\n\n", tol)

	fmt.Printf("Метод простых итераций (Jacobi):\n")
	fmt.Printf("  кол-во итераций: %d\n", itJ)
	fmt.Printf("  решение: [")
	for i := range xJ {
		fmt.Printf(" %8.6f", xJ[i])
	}
	fmt.Println(" ]\n")

	fmt.Printf("Метод Зейделя (Gauss–Seidel):\n")
	fmt.Printf("  кол-во итераций: %d\n", itS)
	fmt.Printf("  решение: [")
	for i := range xS {
		fmt.Printf(" %8.6f", xS[i])
	}
	fmt.Println(" ]")
}
