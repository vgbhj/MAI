package main

import (
	"fmt"
	"math"
)

const eps = 1e-12 // Точность для сравнения чисел с плавающей точкой

// LU-разложение с выбором главного элемента. Возвращает L, U, P, sign, err
func LUPDecomposition(A [][]float64) ([][]float64, [][]float64, [][]int, int, error) {
	n := len(A)
	P := make([][]int, n) // Матрица перестановок
	for i := range P {
		P[i] = make([]int, n)
		P[i][i] = 1
	}
	sign := 1 // Знак определителя

	// Копируем матрицу A
	LU := make([][]float64, n)
	for i := range A {
		LU[i] = make([]float64, n)
		copy(LU[i], A[i])
	}

	for k := 0; k < n; k++ {
		// Выбор главного элемента
		maxRow := k
		maxVal := math.Abs(LU[k][k])
		for i := k + 1; i < n; i++ {
			if val := math.Abs(LU[i][k]); val > maxVal {
				maxVal = val
				maxRow = i
			}
		}

		// Перестановка строк
		if maxRow != k {
			LU[k], LU[maxRow] = LU[maxRow], LU[k]
			P[k], P[maxRow] = P[maxRow], P[k]
			sign *= -1
		}

		if math.Abs(LU[k][k]) < eps {
			return nil, nil, nil, 0, fmt.Errorf("матрица вырождена")
		}

		// Исключение Гаусса
		for i := k + 1; i < n; i++ {
			LU[i][k] /= LU[k][k]
			for j := k + 1; j < n; j++ {
				LU[i][j] -= LU[i][k] * LU[k][j]
			}
		}
	}

	// Разделяем L и U
	L := make([][]float64, n)
	U := make([][]float64, n)
	for i := 0; i < n; i++ {
		L[i] = make([]float64, n)
		U[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			if i > j {
				L[i][j] = LU[i][j]
			} else {
				U[i][j] = LU[i][j]
			}
			if i == j {
				L[i][j] = 1
			}
		}
	}

	return L, U, P, sign, nil
}

// Решение системы Ly = Pb
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

// Решение системы Ux = y
func solveU(U [][]float64, y []float64) []float64 {
	n := len(U)
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		x[i] = y[i]
		for j := i + 1; j < n; j++ {
			x[i] -= U[i][j] * x[j]
		}
		x[i] /= U[i][i]
	}
	return x
}

// Вычисление определителя
func determinant(U [][]float64, sign int) float64 {
	det := 1.0
	for i := range U {
		det *= U[i][i]
	}
	return det * float64(sign)
}

// Вычисление обратной матрицы
func inverseMatrix(A [][]float64) ([][]float64, error) {
	n := len(A)
	inv := make([][]float64, n)
	for i := range inv {
		inv[i] = make([]float64, n)
	}

	L, U, P, _, err := LUPDecomposition(A)
	if err != nil {
		return nil, err
	}

	E := make([]float64, n)
	for col := 0; col < n; col++ {
		// Формируем вектор правой части (столбец единичной матрицы)
		for i := 0; i < n; i++ {
			E[i] = 0.0
		}
		E[col] = 1.0

		// Применяем перестановки
		Pb := make([]float64, n)
		for i := 0; i < n; i++ {
			Pb[i] = E[P[i][i]]
		}

		// Решаем системы
		y := solveL(L, Pb)
		x := solveU(U, y)

		// Записываем результат в обратную матрицу
		for i := 0; i < n; i++ {
			inv[i][col] = x[i]
		}
	}

	return inv, nil
}

func main() {
	// Исходная матрица
	A := [][]float64{
		{-5, -1, -3, -1},
		{-2, 0, 8, -4},
		{-7, -2, 2, -2},
		{2, -4, -4, 4},
	}

	// LU-разложение
	L, U, P, sign, err := LUPDecomposition(A)
	if err != nil {
		fmt.Println("Ошибка:", err)
		return
	}

	// Вывод результатов
	fmt.Println("Матрица L:")
	printMatrix(L)
	fmt.Println("\nМатрица U:")
	printMatrix(U)

	// Решение СЛАУ
	b := []float64{18, -12, 6, -12}
	Pb := applyPermutation(P, b)
	y := solveL(L, Pb)
	x := solveU(U, y)
	fmt.Println("\nРешение СЛАУ:")
	for i, val := range x {
		fmt.Printf("x%d = %.4f\n", i+1, val)
	}

	// Определитель
	det := determinant(U, sign)
	fmt.Printf("\nОпределитель матрицы: %.2f\n", det)

	// Обратная матрица
	inv, err := inverseMatrix(A)
	if err != nil {
		fmt.Println("Ошибка:", err)
		return
	}
	fmt.Println("\nОбратная матрица:")
	printMatrix(inv)
}

// Вспомогательные функции
func printMatrix(m [][]float64) {
	for _, row := range m {
		for _, val := range row {
			fmt.Printf("%8.4f", val)
		}
		fmt.Println()
	}
}

func applyPermutation(P [][]int, b []float64) []float64 {
	n := len(b)
	result := make([]float64, n)
	for i := 0; i < n; i++ {
		result[i] = b[P[i][i]]
	}
	return result
}
