package main

import (
	"fmt"
	"log"
)

// createZeroMatrix создает нулевую матрицу размером n x m
func createZeroMatrix(n, m int) [][]float64 {
	mat := make([][]float64, n)
	for i := 0; i < n; i++ {
		mat[i] = make([]float64, m)
	}
	return mat
}

// createIdentityMatrix создает единичную матрицу n x n
func createIdentityMatrix(n int) [][]float64 {
	I := createZeroMatrix(n, n)
	for i := 0; i < n; i++ {
		I[i][i] = 1
	}
	return I
}

// swapRows меняет местами две строки i и j в матрице M
func swapRows(M [][]float64, i, j int) {
	M[i], M[j] = M[j], M[i]
}

// abs возвращает абсолютное значение
func abs(a float64) float64 {
	if a < 0 {
		return -a
	}
	return a
}

// pivotMatrix создает матрицу перестановок для частичного выбора главного элемента
func pivotMatrix(A [][]float64) [][]float64 {
	m := len(A)
	P := createIdentityMatrix(m)
	for j := 0; j < m; j++ {
		maxRow := j
		maxVal := abs(A[j][j])
		for i := j + 1; i < m; i++ {
			if abs(A[i][j]) > maxVal {
				maxVal = abs(A[i][j])
				maxRow = i
			}
		}
		if maxRow != j {
			swapRows(P, j, maxRow)
		}
	}
	return P
}

// matrixMultiply умножает матрицы A (n x p) и B (p x m)
func matrixMultiply(A, B [][]float64) [][]float64 {
	n := len(A)
	p := len(B)
	m := len(B[0])
	C := createZeroMatrix(n, m)
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			sum := 0.0
			for k := 0; k < p; k++ {
				sum += A[i][k] * B[k][j]
			}
			C[i][j] = sum
		}
	}
	return C
}

// toColMatrix преобразует срез b в матрицу n x 1
func toColMatrix(b []float64) [][]float64 {
	n := len(b)
	col := createZeroMatrix(n, 1)
	for i := 0; i < n; i++ {
		col[i][0] = b[i]
	}
	return col
}

// luDecomposition выполняет LU-разложение A = P^T L U
func luDecomposition(A [][]float64) (P, L, U [][]float64) {
	n := len(A)
	L = createZeroMatrix(n, n)
	U = createZeroMatrix(n, n)
	P = pivotMatrix(A)
	PA := matrixMultiply(P, A)

	for j := 0; j < n; j++ {
		L[j][j] = 1
		// U
		for i := 0; i <= j; i++ {
			s := 0.0
			for k := 0; k < i; k++ {
				s += U[k][j] * L[i][k]
			}
			U[i][j] = PA[i][j] - s
		}
		// L
		for i := j; i < n; i++ {
			s := 0.0
			for k := 0; k < j; k++ {
				s += U[k][j] * L[i][k]
			}
			L[i][j] = (PA[i][j] - s) / U[j][j]
		}
	}
	return P, L, U
}

// solveL решает L y = Pb прямой подстановкой
func solveL(L [][]float64, Pb []float64) []float64 {
	n := len(L)
	y := make([]float64, n)
	for i := 0; i < n; i++ {
		y[i] = Pb[i]
		for j := 0; j < i; j++ {
			y[i] -= L[i][j] * y[j]
		}
	}
	return y
}

// solveU решает U x = y обратной подстановкой
func solveU(U [][]float64, y []float64) []float64 {
	n := len(U)
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		s := y[i]
		for j := i + 1; j < n; j++ {
			s -= U[i][j] * x[j]
		}
		if U[i][i] == 0 {
			log.Fatalf("solveU: нулевой диагональный элемент U[%d][%d]", i, i)
		}
		x[i] = s / U[i][i]
	}
	return x
}

// luSolve решает Ax = b через LU-разложение
func luSolve(L, U, P [][]float64, b []float64) []float64 {
	n := len(L)
	PbMat := matrixMultiply(P, toColMatrix(b))
	Pb := make([]float64, n)
	for i := 0; i < n; i++ {
		Pb[i] = PbMat[i][0]
	}
	y := solveL(L, Pb)
	x := solveU(U, y)
	return x
}

// luInverse вычисляет обратную матрицу через LU
func luInverse(L, U, P [][]float64) [][]float64 {
	n := len(L)
	invA := createZeroMatrix(n, n)
	for i := 0; i < n; i++ {
		e := make([]float64, n)
		e[i] = 1
		col := luSolve(L, U, P, e)
		for j := 0; j < n; j++ {
			invA[j][i] = col[j]
		}
	}
	return invA
}

// determinant вычисляет det(A) через U и P
func determinant(P, U [][]float64) float64 {
	n := len(U)
	swaps := 0
	for i := 0; i < n; i++ {
		if P[i][i] != 1 {
			swaps++
		}
	}
	sign := 1.0
	if swaps%2 != 0 {
		sign = -1
	}
	det := sign
	for i := 0; i < n; i++ {
		det *= U[i][i]
	}
	return det
}

// printMatrix выводит матрицу
func printMatrix(M [][]float64) {
	for _, row := range M {
		for _, v := range row {
			fmt.Printf("%8.4f ", v)
		}
		fmt.Println()
	}
}

// printSlice выводит срез чисел
func printSlice(v []float64) {
	for _, val := range v {
		fmt.Printf("%8.4f ", val)
	}
	fmt.Println()
}

func main() {
	A := [][]float64{
		{-5, -1, -3, -1},
		{-2, 0, 8, -4},
		{-7, -2, 2, -2},
		{2, -4, -4, 4},
	}
	b := []float64{18, -12, 6, -12}

	// LU-разложение
	P, L, U := luDecomposition(A)

	// Решение системы
	x := luSolve(L, U, P, b)

	// Обратная матрица
	invA := luInverse(L, U, P)

	// Детерминант
	detA := determinant(P, U)

	// Дополнительные матрицы
	PA := matrixMultiply(P, A)
	LU := matrixMultiply(L, U)
	PLU := matrixMultiply(P, LU)
	Acheck := matrixMultiply(invA, A)

	// Вывод результатов
	fmt.Println("Матрица перестановок P:")
	printMatrix(P)

	fmt.Println("\nМатрица PA (P * A):")
	printMatrix(PA)

	fmt.Println("\nМатрица L:")
	printMatrix(L)

	fmt.Println("\nМатрица U:")
	printMatrix(U)

	fmt.Println("\nМатрица LU (L * U):")
	printMatrix(LU)

	fmt.Println("\nМатрица PLU (P * L * U):")
	printMatrix(PLU)

	fmt.Println("\nРешение системы:")
	printSlice(x)

	fmt.Println("\nОпределитель:", detA)

	fmt.Println("\nОбратная матрица:")
	printMatrix(invA)

	fmt.Println("\nПроверка (Обратная матрица * A = Единичная матрица):")
	printMatrix(Acheck)
}
