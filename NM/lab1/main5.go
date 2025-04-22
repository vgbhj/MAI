package main

import (
	"fmt"
	"math"
	"os"
)

// qrDecompose выполняет классическое разложение A = Q·R,
// где A — произвольная квадратная матрица n×n.
// Возвращает Q и R как двумерные срезы.
func qrDecompose(A [][]float64) (Q, R [][]float64) {
	n := len(A)
	// инициализируем Q и R
	Q = make([][]float64, n)
	R = make([][]float64, n)
	for i := 0; i < n; i++ {
		Q[i] = make([]float64, n)
		R[i] = make([]float64, n)
	}

	// классический Грама–Шмидт по столбцам
	for j := 0; j < n; j++ {
		// v = j‑й столбец A
		v := make([]float64, n)
		for i := 0; i < n; i++ {
			v[i] = A[i][j]
		}
		// ортогонализация относительно предыдущих колонок Q
		for i := 0; i < j; i++ {
			// R[i][j] = Q[:,i]·A[:,j]
			dot := 0.0
			for k := 0; k < n; k++ {
				dot += Q[k][i] * A[k][j]
			}
			R[i][j] = dot
			// v -= dot * Q[:,i]
			for k := 0; k < n; k++ {
				v[k] -= dot * Q[k][i]
			}
		}
		// R[j][j] = ||v||
		norm := 0.0
		for k := 0; k < n; k++ {
			norm += v[k] * v[k]
		}
		norm = math.Sqrt(norm)
		if norm == 0 {
			fmt.Fprintf(os.Stderr, "qrDecompose: нулевая норма на шаге %d\n", j)
			os.Exit(1)
		}
		R[j][j] = norm
		// Q[:,j] = v / norm
		for k := 0; k < n; k++ {
			Q[k][j] = v[k] / norm
		}
	}
	return
}

// offDiagonalNorm возвращает max_{i≠j} |A[i][j]|.
func offDiagonalNorm(A [][]float64) float64 {
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

// qrEigenvalues находит собственные значения произвольной (возможно несимметричной)
// матрицы A методом простого QR‑алгоритма без сдвигов.
// eps — требуемая точность по вне­диагонали (||Aₖ−diag||∞ < eps).
// maxIter — ограничение числа итераций.
// Возвращает приближённые собственные значения (на диагонали A_k),
// и количество итераций.
func qrEigenvalues(A [][]float64, eps float64, maxIter int) ([]float64, int) {
	n := len(A)
	// делаем глубокую копию A в Ak
	Ak := make([][]float64, n)
	for i := range A {
		Ak[i] = append([]float64(nil), A[i]...)
	}

	for k := 1; k <= maxIter; k++ {
		// проверяем off‑диагональ
		off := offDiagonalNorm(Ak)
		fmt.Printf("итерация %3d: off‑diag norm = %.3e\n", k, off)
		if off < eps {
			// собственные значения на диагонали
			vals := make([]float64, n)
			for i := 0; i < n; i++ {
				vals[i] = Ak[i][i]
			}
			return vals, k
		}
		// QR‑разложение
		Q, R := qrDecompose(Ak)
		// следующий Ak = R·Q
		next := make([][]float64, n)
		for i := 0; i < n; i++ {
			next[i] = make([]float64, n)
			for j := 0; j < n; j++ {
				sum := 0.0
				for t := 0; t < n; t++ {
					sum += R[i][t] * Q[t][j]
				}
				next[i][j] = sum
			}
		}
		Ak = next
	}

	fmt.Fprintln(os.Stderr, "qrEigenvalues: не сошлось за maxIter")
	// возвращаем диагональ Ak как есть
	vals := make([]float64, n)
	for i := 0; i < n; i++ {
		vals[i] = Ak[i][i]
	}
	return vals, maxIter
}

func main() {
	A := [][]float64{
		{1, 2, 5},
		{-8, 0, -6},
		{7, -9, -7},
	}

	eps := 1e-6
	maxIter := 100

	fmt.Println("Запускаем QR‑алгоритм без сдвигов для 3×3 матрицы")
	vals, it := qrEigenvalues(A, eps, maxIter)

	fmt.Printf("\nСобственных итераций: %d\n", it)
	fmt.Println("Приближённые собственные значения:")
	for i, λ := range vals {
		fmt.Printf("  λ%v = %9.6f\n", i+1, λ)
	}
}
