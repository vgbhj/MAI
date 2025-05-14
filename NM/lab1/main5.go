package main

import (
	"fmt"
	"math"
	"math/cmplx"
)

const eps = 1e-6

func sign(x float64) float64 {
	if x >= 0 {
		return 1
	}
	return -1
}

func norm(vec []float64) float64 {
	sum := 0.0
	for _, v := range vec {
		sum += v * v
	}
	return math.Sqrt(sum)
}

func dot(a, b []float64) float64 {
	sum := 0.0
	for i := range a {
		sum += a[i] * b[i]
	}
	return sum
}

func householderMatrix(A [][]float64, col int) [][]float64 {
	n := len(A)
	v := make([]float64, n)
	a := make([]float64, n)
	for i := range A {
		a[i] = A[i][col]
	}

	sigma := sign(a[col]) * norm(a[col:])
	v[col] = a[col] + sigma
	for i := col + 1; i < n; i++ {
		v[i] = a[i]
	}

	beta := dot(v, v)
	if beta == 0 {
		return identityMatrix(n)
	}

	H := identityMatrix(n)
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			H[i][j] -= 2 * v[i] * v[j] / beta
		}
	}
	return H
}

func qrDecompose(A [][]float64) ([][]float64, [][]float64) {
	n := len(A)
	Q := identityMatrix(n)
	A_i := copyMatrix(A)

	for i := 0; i < n-1; i++ {
		H := householderMatrix(A_i, i)
		Q = multiplyMatrices(Q, H)
		A_i = multiplyMatrices(H, A_i)
	}
	return Q, A_i
}

func getRoots(A [][]float64, i int) []complex128 {
	a11 := A[i][i]
	a12 := A[i][i+1]
	a21 := A[i+1][i]
	a22 := A[i+1][i+1]

	tr := a11 + a22
	det := a11*a22 - a12*a21
	discriminant := tr*tr - 4*det

	if discriminant >= 0 {
		sqrtD := math.Sqrt(discriminant)
		return []complex128{
			complex((-tr+sqrtD)/2, 0),
			complex((-tr-sqrtD)/2, 0),
		}
	}
	sqrtD := cmplx.Sqrt(complex(discriminant, 0))
	return []complex128{
		(-complex(tr, 0) + sqrtD) / 2,
		(-complex(tr, 0) - sqrtD) / 2,
	}
}

func isComplexBlock(A [][]float64, i int, eps float64) bool {
	Q, R := qrDecompose(A)
	A_next := multiplyMatrices(R, Q)
	roots1 := getRoots(A, i)
	roots2 := getRoots(A_next, i)

	return cmplx.Abs(roots1[0]-roots2[0]) < eps &&
		cmplx.Abs(roots1[1]-roots2[1]) < eps
}

func qrEigenValues(A [][]float64, eps float64) []complex128 {
	n := len(A)
	A_i := copyMatrix(A)
	eigenValues := make([]complex128, 0, n)
	i := 0

	for i < n {
		if i == n-1 {
			eigenValues = append(eigenValues, complex(A_i[i][i], 0))
			break
		}

		subNorm := norm([]float64{A_i[i+1][i]})
		if subNorm < eps {
			eigenValues = append(eigenValues, complex(A_i[i][i], 0))
			i++
		} else {
			if isComplexBlock(A_i, i, eps) {
				roots := getRoots(A_i, i)
				eigenValues = append(eigenValues, roots...)
				i += 2
			} else {
				Q, R := qrDecompose(A_i)
				A_i = multiplyMatrices(R, Q)
			}
		}
	}
	return eigenValues
}

// Вспомогательные функции
func identityMatrix(n int) [][]float64 {
	m := make([][]float64, n)
	for i := range m {
		m[i] = make([]float64, n)
		m[i][i] = 1
	}
	return m
}

func copyMatrix(A [][]float64) [][]float64 {
	m := make([][]float64, len(A))
	for i := range A {
		m[i] = append([]float64{}, A[i]...)
	}
	return m
}

func multiplyMatrices(A, B [][]float64) [][]float64 {
	n := len(A)
	result := make([][]float64, n)
	for i := range result {
		result[i] = make([]float64, n)
		for j := range result[i] {
			sum := 0.0
			for k := 0; k < n; k++ {
				sum += A[i][k] * B[k][j]
			}
			result[i][j] = sum
		}
	}
	return result
}
func main() {
	A := [][]float64{
		{1, 2, 5},
		{-8, 0, -6},
		{7, -9, -7},
	}

	// 	A := [][]float64{
	//     {9, 0, 2},
	//     {-6, 4, 4},
	//     {-2, -7, 5},
	// }

	eigenvalues := qrEigenValues(A, eps)
	fmt.Println("Собственные значения:", eigenvalues)
}
