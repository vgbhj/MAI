package main

import (
	"encoding/csv"
	"fmt"
	"log"
	"math"
	"os"
	"strconv"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/vg"
)

// Проверка симметричности
func isSymmetric(A [][]float64, tol float64) bool {
	n := len(A)
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			if math.Abs(A[i][j]-A[j][i]) > tol {
				return false
			}
		}
	}
	return true
}

// offNorm возвращает ∞‑норму вектора всех вне­диагональных элементов A
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

func jacobiEigen(A [][]float64, eps float64, maxIter int) (D, U [][]float64, it int, errors []float64) {
	if !isSymmetric(A, 1e-9) {
		log.Fatalln("Матрица не симметрична")
	}

	n := len(A)
	D = make([][]float64, n)
	for i := range D {
		D[i] = make([]float64, n)
		copy(D[i], A[i])
	}
	U = make([][]float64, n)
	for i := range U {
		U[i] = make([]float64, n)
		U[i][i] = 1.0
	}

	for k := 1; k <= maxIter; k++ {
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

		errors = append(errors, maxv)
		fmt.Printf("Итерация %2d: off-norm = %.6e\n", k, maxv)
		if maxv < eps {
			it = k
			return
		}

		phi := 0.5 * math.Atan2(2*D[p][q], D[q][q]-D[p][p])
		c := math.Cos(phi)
		s := math.Sin(phi)

		Dpp := c*c*D[p][p] - 2*s*c*D[p][q] + s*s*D[q][q]
		Dqq := s*s*D[p][p] + 2*s*c*D[p][q] + c*c*D[q][q]
		Dpq := 0.0

		for i := 0; i < n; i++ {
			if i != p && i != q {
				Dip := c*D[i][p] - s*D[i][q]
				Diq := s*D[i][p] + c*D[i][q]
				D[i][p], D[p][i] = Dip, Dip
				D[i][q], D[q][i] = Diq, Diq
			}
		}
		D[p][p], D[q][q], D[p][q], D[q][p] = Dpp, Dqq, Dpq, Dpq

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

func saveCSV(errors []float64, filename string) {
	file, err := os.Create(filename)
	if err != nil {
		log.Fatalf("Ошибка создания CSV: %v", err)
	}
	defer file.Close()

	writer := csv.NewWriter(file)
	defer writer.Flush()

	writer.Write([]string{"iteration", "error"})
	for i, val := range errors {
		writer.Write([]string{strconv.Itoa(i + 1), fmt.Sprintf("%e", val)})
	}
}

func plotError(errors []float64, filename string) {
	pts := make(plotter.XYs, len(errors))
	for i, val := range errors {
		pts[i].X = float64(i + 1)
		pts[i].Y = val
	}

	p := plot.New()
	p.Title.Text = "Сходимость метода вращений (off-norm)"
	p.X.Label.Text = "Итерация"
	p.Y.Label.Text = "Погрешность (off-norm)"
	p.Y.Scale = plot.LogScale{}
	p.Y.Min = 1e-9

	errLine, _ := plotter.NewLine(pts)
	p.Add(errLine)

	if err := p.Save(6*vg.Inch, 4*vg.Inch, filename); err != nil {
		log.Fatalf("Ошибка сохранения графика: %v", err)
	}
}

// Норма ||UᵀU - I||
func orthonormalityError(U [][]float64) float64 {
	n := len(U)
	err := 0.0
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			dot := 0.0
			for k := 0; k < n; k++ {
				dot += U[k][i] * U[k][j]
			}
			expected := 0.0
			if i == j {
				expected = 1.0
			}
			err = math.Max(err, math.Abs(dot-expected))
		}
	}
	return err
}

// Норма ||UDUᵀ - A||
func reconstructionError(A, U, D [][]float64) float64 {
	n := len(A)
	recon := make([][]float64, n)
	for i := range recon {
		recon[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			for k := 0; k < n; k++ {
				for l := 0; l < n; l++ {
					recon[i][j] += U[i][k] * D[k][l] * U[j][l]
				}
			}
		}
	}

	err := 0.0
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			err = math.Max(err, math.Abs(recon[i][j]-A[i][j]))
		}
	}
	return err
}

func main() {
	A := [][]float64{
		{8, -3, 9},
		{-3, 8, -2},
		{9, -2, -8},
	}

	eps := 1e-6
	maxIter := 100

	D, U, it, errors := jacobiEigen(A, eps, maxIter)

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

	// Численные проверки
	fmt.Println("\nПроверка ортонормированности (||UᵀU - I||):")
	orthErr := orthonormalityError(U)
	fmt.Printf("  Ошибка ортонормированности: %.3e\n", orthErr)

	fmt.Println("Проверка реконструкции (||UDUᵀ - A||):")
	reconErr := reconstructionError(A, U, D)
	fmt.Printf("  Ошибка реконструкции:       %.3e\n", reconErr)

	// Сохраняем и строим график
	saveCSV(errors, "error.csv")
	plotError(errors, "error.png")
	fmt.Println("\nCSV сохранён в error.csv, график — в error.png")
}
