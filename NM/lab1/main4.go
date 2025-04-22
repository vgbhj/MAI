package main

import (
	"fmt"
	"math"
	"os"
)

// offNorm возвращает ∞‑норму вектора всех вне­диагональных элементов A:
// max_{i≠j} |A[i][j]|
func offNorm(A [][]float64) float64 {
	n := len(A)
	maxv := 0.0
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			if i != j {
				if v := math.Abs(A[i][j]); v > maxv {
					maxv = v
				}
			}
		}
	}
	return maxv
}

// jacobiEigen реализует метод вращений.
// A — исходная симметрическая матрица (матрица копируется),
// eps — требуемая точность по вне­диагонали,
// maxIter — максимальное число итераций.
// Возвращает:
//
//	D  — диагонализованную матрицу (соб. значения на диагонали),
//	U  — матрицу собственных векторов (столбцы),
//	it — число фактически выполненных итераций.
func jacobiEigen(A [][]float64, eps float64, maxIter int) (D, U [][]float64, it int) {
	n := len(A)
	// копируем A в D
	D = make([][]float64, n)
	for i := range D {
		D[i] = make([]float64, n)
		copy(D[i], A[i])
	}
	// U = I
	U = make([][]float64, n)
	for i := range U {
		U[i] = make([]float64, n)
		U[i][i] = 1.0
	}

	for k := 1; k <= maxIter; k++ {
		// находим индексы максимума |D[p][q]|, p<q
		p, q := 0, 1
		maxv := math.Abs(D[0][1])
		for i := 0; i < n; i++ {
			for j := i + 1; j < n; j++ {
				if v := math.Abs(D[i][j]); v > maxv {
					maxv = v
					p, q = i, j
				}
			}
		}

		// выводим off‑norm
		fmt.Printf("Итерация %2d: off-norm = %.6e\n", k, maxv)
		if maxv < eps {
			it = k
			return
		}

		// считаем угол φ
		phi := 0.5 * math.Atan2(2*D[p][q], D[q][q]-D[p][p])
		c := math.Cos(phi)
		s := math.Sin(phi)

		// делаем вращение над парами (p,q)
		Dpp := c*c*D[p][p] - 2*s*c*D[p][q] + s*s*D[q][q]
		Dqq := s*s*D[p][p] + 2*s*c*D[p][q] + c*c*D[q][q]
		Dpq := 0.0

		// обновляем строки/столбцы кроме p,q
		for i := 0; i < n; i++ {
			if i != p && i != q {
				Dip := c*D[i][p] - s*D[i][q]
				Diq := s*D[i][p] + c*D[i][q]
				D[i][p], D[p][i] = Dip, Dip
				D[i][q], D[q][i] = Diq, Diq
			}
		}
		D[p][p], D[q][q], D[p][q], D[q][p] = Dpp, Dqq, Dpq, Dpq

		// аккумулируем матрицу U
		for i := 0; i < n; i++ {
			Up := c*U[i][p] - s*U[i][q]
			Uq := s*U[i][p] + c*U[i][q]
			U[i][p], U[i][q] = Up, Uq
		}
	}

	fmt.Fprintln(os.Stderr, "достигнут maxIter без сходимости")
	it = maxIter
	return
}

func main() {
	A := [][]float64{
		{8, -3, 9},
		{-3, 8, -2},
		{9, -2, -8},
	}

	eps := 1e-6
	maxIter := 100

	D, U, it := jacobiEigen(A, eps, maxIter)

	fmt.Println("\n=== Результат ===")
	fmt.Printf("Собственных итераций выполнено: %d\n\n", it)

	fmt.Println("Собственные значения (на диагонали D):")
	for i := range D {
		fmt.Printf(" λ%v = %9.6f\n", i+1, D[i][i])
	}

	fmt.Println("\nСобственные векторы (столбцы U):")
	for i := 0; i < len(U); i++ {
		fmt.Printf("v%v = [", i+1)
		for j := 0; j < len(U); j++ {
			fmt.Printf(" %8.6f", U[j][i])
		}
		fmt.Println(" ]")
	}
}
