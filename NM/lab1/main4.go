package main

import (
	"fmt"
	"log"
	"math"
	"os"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/plotutil"
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

func plotError(errors []float64, filename string) {
	pts := make(plotter.XYs, len(errors))
	for i, val := range errors {
		pts[i].X = float64(i + 1)
		pts[i].Y = val
	}

	p := plot.New()
	p.Title.Text = "Сходимость метода вращений Якоби"
	p.X.Label.Text = "Итерация"
	p.Y.Label.Text = "Макс. внедиагональный элемент (log)"

	// Оси и стиль
	p.Y.Scale = plot.LogScale{}
	p.Y.Tick.Marker = plot.LogTicks{}
	p.Add(plotter.NewGrid())

	line, err := plotter.NewLine(pts)
	if err != nil {
		log.Fatalf("Ошибка создания линии графика: %v", err)
	}
	line.LineStyle.Width = vg.Points(2)
	line.LineStyle.Color = plotutil.Color(1)

	points, err := plotter.NewScatter(pts)
	if err != nil {
		log.Fatalf("Ошибка создания точек графика: %v", err)
	}
	points.Shape = plotutil.Shape(1)
	points.Radius = vg.Points(2)
	points.Color = plotutil.Color(0)

	p.Add(line, points)

	if err := p.Save(8*vg.Inch, 5*vg.Inch, filename); err != nil {
		log.Fatalf("Ошибка сохранения графика: %v", err)
	}
}

// Проверка Av ≈ λv для всех собственных векторов
func checkEigen(A, U, D [][]float64, tol float64) {
	n := len(A)
	fmt.Println("\nПроверка A*v_i = λ_i*v_i:")
	for i := 0; i < n; i++ {
		// Получаем i-й собственный вектор (i-й столбец U)
		lambda := D[i][i]
		v := make([]float64, n)
		for j := 0; j < n; j++ {
			v[j] = U[j][i]
		}

		// Вычисляем A*v
		Av := make([]float64, n)
		for row := 0; row < n; row++ {
			for col := 0; col < n; col++ {
				Av[row] += A[row][col] * v[col]
			}
		}

		// Вычисляем λ*v
		lv := make([]float64, n)
		for j := 0; j < n; j++ {
			lv[j] = lambda * v[j]
		}

		// Сравниваем результаты
		maxError := 0.0
		for j := 0; j < n; j++ {
			err := math.Abs(Av[j] - lv[j])
			if err > maxError {
				maxError = err
			}
		}

		fmt.Printf("λ_%d = %9.6f\n", i+1, lambda)
		fmt.Printf("  A*v_%d = [", i+1)
		for _, val := range Av {
			fmt.Printf(" %8.6f", val)
		}
		fmt.Println(" ]")
		fmt.Printf("  λ*v_%d = [", i+1)
		for _, val := range lv {
			fmt.Printf(" %8.6f", val)
		}
		fmt.Println(" ]")

		if maxError < tol {
			fmt.Printf("Проверка пройдена (макс. ошибка: %.2e)\n\n", maxError)
		} else {
			fmt.Printf("Ошибка: максимальная погрешность %.2e превышает допуск\n\n", maxError)
		}
	}
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

	fmt.Println("\n=== Проверки ===")
	checkEigen(A, U, D, eps)

	// Сохраняем и строим график
	plotError(errors, "error.png")
}
