package main

import (
	"fmt"
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
			xNew[i] = (b[i] - sigma) / A[i][i]
		}

		// Вычисляем евклидову норму разности
		sumSq := 0.0
		for i := 0; i < n; i++ {
			d := xNew[i] - x[i]
			sumSq += d * d
		}
		diff := math.Sqrt(sumSq)
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
			for j := 0; j < i; j++ {
				sigma += A[i][j] * x[j]
			}
			for j := i + 1; j < n; j++ {
				sigma += A[i][j] * xOld[j]
			}
			x[i] = (b[i] - sigma) / A[i][i]
		}
		// СДелать как в методичке
		// Вычисляем евклидову норму разности
		sumSq := 0.0
		for i := 0; i < n; i++ {
			d := x[i] - xOld[i]
			sumSq += d * d
		}
		diff := math.Sqrt(sumSq)

		if diff < tol {
			return x, k
		}
	}
	return x, maxIter
}

func checkSolution(A [][]float64, x []float64, b []float64) {
	fmt.Println("Проверка A * x ≈ b:")
	for i := 0; i < len(b); i++ {
		sum := 0.0
		for j := 0; j < len(x); j++ {
			sum += A[i][j] * x[j]
		}
		fmt.Printf("  строка %d: %.6f ≈ %.6f\n", i+1, sum, b[i])
	}
	fmt.Println()
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
	fmt.Println(" ]")
	checkSolution(A, xJ, b)

	fmt.Printf("Метод Зейделя (Gauss–Seidel):\n")
	fmt.Printf("  кол-во итераций: %d\n", itS)
	fmt.Printf("  решение: [")
	for i := range xS {
		fmt.Printf(" %8.6f", xS[i])
	}
	fmt.Println(" ]")
	checkSolution(A, xS, b)

}
