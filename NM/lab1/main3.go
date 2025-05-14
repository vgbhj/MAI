package main

import (
	"fmt"
	"math"
	"strings"
)

// calculateL2Norm computes the Euclidean norm of a vector.
func calculateL2Norm(v []float64) float64 {
	sum := 0.0
	for _, x := range v {
		sum += x * x
	}
	return math.Sqrt(sum)
}

// simpleIterationMethod solves Ax = b by simple iteration.
func simpleIterationMethod(A [][]float64, b []float64, epsilon float64, maxIter int) ([]float64, int) {
	n := len(A)
	alpha := make([][]float64, n)
	beta := make([]float64, n)
	for i := 0; i < n; i++ {
		alpha[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			if i != j {
				alpha[i][j] = -A[i][j] / A[i][i]
			}
		}
		beta[i] = b[i] / A[i][i]
	}
	xNew := append([]float64(nil), beta...)
	alphaNorm := calculateL2Norm(flatten(alpha))
	var err float64
	var iter int

	for iter = 0; iter < maxIter; iter++ {
		xPrev := append([]float64(nil), xNew...)
		for i := 0; i < n; i++ {
			sum := 0.0
			for j := 0; j < n; j++ {
				sum += alpha[i][j] * xPrev[j]
			}
			xNew[i] = sum + beta[i]
		}
		if alphaNorm >= 1 {
			err = calculateL2Norm(diff(xNew, xPrev))
		} else {
			err = (alphaNorm / (1 - alphaNorm)) * calculateL2Norm(diff(xNew, xPrev))
		}
		if err <= epsilon {
			break
		}
	}
	return xNew, iter + 1
}

// createPermutationMatrix returns permutation matrix and swap count for partial pivoting.
func createPermutationMatrix(M [][]float64) ([][]float64, int) {
	n := len(M)
	P := identity(n)
	swaps := 0
	for i := 0; i < n; i++ {
		maxIdx := i
		maxVal := math.Abs(M[i][i])
		for k := i + 1; k < n; k++ {
			if math.Abs(M[k][i]) > maxVal {
				maxVal = math.Abs(M[k][i])
				maxIdx = k
			}
		}
		if maxIdx != i {
			P[i], P[maxIdx] = P[maxIdx], P[i]
			swaps++
		}
	}
	return P, swaps
}

// lu decomposes permuted matrix into L and U.
func lu(PA [][]float64) ([][]float64, [][]float64) {
	n := len(PA)
	L := make([][]float64, n)
	U := make([][]float64, n)
	for i := range PA {
		L[i] = make([]float64, n)
		U[i] = append([]float64(nil), PA[i]...)
		L[i][i] = 1
	}
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			if U[i][i] == 0 {
				continue
			}
			L[j][i] = U[j][i] / U[i][i]
			for k := i; k < n; k++ {
				U[j][k] -= L[j][i] * U[i][k]
			}
		}
	}
	return L, U
}

// solveLinearSystem performs forward and backward substitution.
func solveLinearSystem(L, U [][]float64, b []float64) []float64 {
	n := len(L)
	y := make([]float64, n)
	for i := 0; i < n; i++ {
		sum := 0.0
		for j := 0; j < i; j++ {
			sum += L[i][j] * y[j]
		}
		y[i] = (b[i] - sum) / L[i][i]
	}
	x := make([]float64, n)
	for i := n - 1; i >= 0; i-- {
		sum := 0.0
		for j := i + 1; j < n; j++ {
			sum += U[i][j] * x[j]
		}
		x[i] = (y[i] - sum) / U[i][i]
	}
	return x
}

// transpose returns the transpose of a matrix.
func transpose(M [][]float64) [][]float64 {
	n := len(M)
	m := len(M[0])
	T := make([][]float64, m)
	for i := range T {
		T[i] = make([]float64, n)
		for j := range T[i] {
			T[i][j] = M[j][i]
		}
	}
	return T
}

// computeInverse returns the inverse of a matrix.
func computeInverse(M [][]float64) [][]float64 {
	n := len(M)
	I := identity(n)
	P, _ := createPermutationMatrix(M)
	PA := multiply(P, M)
	L, U := lu(PA)
	inv := make([][]float64, n)
	for i := 0; i < n; i++ {
		col := make([]float64, n)
		for j := 0; j < n; j++ {
			col[j] = I[j][i]
		}
		b := multiplyMatrixVector(P, col)
		x := solveLinearSystem(L, U, b)
		inv[i] = x
	}
	return transpose(inv)
}

// seidelMethod solves Ax=b by Gauss-Seidel iteration.
func seidelMethod(A [][]float64, b []float64, epsilon float64, maxIter int) ([]float64, int) {
	n := len(A)
	alpha := make([][]float64, n)
	beta := make([]float64, n)
	for i := range A {
		alpha[i] = make([]float64, n)
		for j := range A {
			if i != j {
				alpha[i][j] = -A[i][j] / A[i][i]
			}
		}
		beta[i] = b[i] / A[i][i]
	}
	// Split alpha into B (lower) and C (upper)
	B := make([][]float64, n)
	C := make([][]float64, n)
	for i := range alpha {
		B[i] = make([]float64, n)
		C[i] = make([]float64, n)
		for j := range alpha {
			if j < i {
				B[i][j] = alpha[i][j]
			} else {
				C[i][j] = alpha[i][j]
			}
		}
	}
	EminusB := make([][]float64, n)
	for i := 0; i < n; i++ {
		EminusB[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			if i == j {
				EminusB[i][j] = 1
			} else {
				EminusB[i][j] = -B[i][j]
			}
		}
	}
	invEMB := computeInverse(EminusB)
	tmp1 := multiply(invEMB, C)
	tmp2 := multiplyMatrixVector(invEMB, beta)
	xNew := append([]float64(nil), tmp2...)
	alphaNorm := calculateL2Norm(flatten(alpha))
	var err float64
	var iter int

	for iter = 0; iter < maxIter; iter++ {
		xPrev := append([]float64(nil), xNew...)
		for i := range tmp1 {
			sum := 0.0
			for j := range xPrev {
				sum += tmp1[i][j] * xPrev[j]
			}
			xNew[i] = sum + tmp2[i]
		}
		if alphaNorm >= 1 {
			err = calculateL2Norm(diff(xNew, xPrev))
		} else {
			err = (calculateL2Norm(flatten(C)) / (1 - alphaNorm)) * calculateL2Norm(diff(xNew, xPrev))
		}
		if err <= epsilon {
			break
		}
	}
	return xNew, iter + 1
}

// multiplyMatrixVector computes A*v.
func multiplyMatrixVector(A [][]float64, v []float64) []float64 {
	n := len(A)
	res := make([]float64, n)
	for i := 0; i < n; i++ {
		sum := 0.0
		for j := 0; j < len(v); j++ {
			sum += A[i][j] * v[j]
		}
		res[i] = sum
	}
	return res
}

// multiply performs A*B.
func multiply(A, B [][]float64) [][]float64 {
	n := len(A)
	C := make([][]float64, n)
	for i := 0; i < n; i++ {
		C[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			sum := 0.0
			for k := 0; k < n; k++ {
				sum += A[i][k] * B[k][j]
			}
			C[i][j] = sum
		}
	}
	return C
}

// identity creates an identity matrix.
func identity(n int) [][]float64 {
	I := make([][]float64, n)
	for i := 0; i < n; i++ {
		I[i] = make([]float64, n)
		I[i][i] = 1
	}
	return I
}

// flatten converts 2D slice to 1D.
func flatten(M [][]float64) []float64 {
	var v []float64
	for _, row := range M {
		v = append(v, row...)
	}
	return v
}

// diff returns element-wise difference a - b.
func diff(a, b []float64) []float64 {
	res := make([]float64, len(a))
	for i := range a {
		res[i] = a[i] - b[i]
	}
	return res
}

// formatMatrix prints matrix nicely.
func formatMatrix(M [][]float64) string {
	var sb strings.Builder
	for _, row := range M {
		for _, v := range row {
			if math.Abs(v) < 1e-10 {
				sb.WriteString(fmt.Sprintf("%6.2f ", 0.0))
			} else {
				sb.WriteString(fmt.Sprintf("%6.2f ", v))
			}
		}
		sb.WriteString("\n")
	}
	return sb.String()
}

func main() {
	A := [][]float64{
		{21, -6, -9, -4},
		{-6, 20, -4, 2},
		{-2, -7, 20, 3},
		{4, 9, 6, 24},
	}
	b := []float64{127, -144, -236, 5}
	epsilon := 1e-6

	x1, it1 := simpleIterationMethod(A, b, epsilon, 100)
	x2, it2 := seidelMethod(A, b, epsilon, 100)

	fmt.Println("Matrix A:")
	fmt.Println(formatMatrix(A))
	fmt.Println("Vector b:")
	for _, v := range b {
		fmt.Printf("%6.2f ", v)
	}
	fmt.Println("\n")

	fmt.Println("Simple iterations method:")
	fmt.Println("Solution x:")
	for _, v := range x1 {
		fmt.Printf("%6.2f ", v)
	}
	fmt.Printf("\nNumber of iterations: %d\n\n", it1)
	fmt.Println("Check A * x = b:")
	check1 := multiplyMatrixVector(A, x1)
	for _, v := range check1 {
		fmt.Printf("%6.2f ", v)
	}
	fmt.Println("\n")

	fmt.Println("Seidel method:")
	fmt.Println("Solution x:")
	for _, v := range x2 {
		fmt.Printf("%6.2f ", v)
	}
	fmt.Printf("\nNumber of iterations: %d\n\n", it2)
	fmt.Println("Check A * x = b:")
	check2 := multiplyMatrixVector(A, x2)
	for _, v := range check2 {
		fmt.Printf("%6.2f ", v)
	}
}
