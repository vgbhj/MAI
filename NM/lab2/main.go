package main

import (
	"fmt"
	"math"

	equations "github.com/AntonCkya/numeric_methods/Equations"
	systems "github.com/AntonCkya/numeric_methods/Systems"
    plotter "github.com/AntonCkya/numeric_methods/Plotter"
)

func EquationRunner() {
	// эквивалентная функция x=phi(x)
    PHI := func(x float64) float64 {
		return 0.5 * math.Log2(5*x+2)
		// return math.Log(4-3*x) / 2
	}
	DPHI := func(x float64) float64 {
		return 5 / (2 * math.Log(2) * (5*x + 2))
		// return -3 / (2 * (4 - 3*x))
	}
	F := func(x float64) float64 {
		return math.Pow(4, x) - 5*x - 2
		// return math.Pow(math.E, 2*x) + 3*x - 4
	}
	DF := func(x float64) float64 {
		return math.Pow(4, x)*math.Log(4) - 5
		// return 2*math.Pow(math.E, 2*x) + 3
	}
	D2F := func(x float64) float64 {
		return math.Pow(4, x) * math.Log(4) * math.Log(4)
		// return 4 * math.Pow(math.E, 2*x)
	}
	var xs, ys1, ys2 []float64
    for x := 0.0; x <= 2.5; x += 0.01 {
        xs = append(xs, x)
        ys1 = append(ys1, math.Pow(4, x))
        ys2 = append(ys2, 5*x+2)
    }
    plotter.Plot2(xs, ys1, xs, ys2, "equation_init", []string{"y=4^x", "y=5x+2"})


	fmt.Println("2.1-----Equations-----")
	fmt.Println("Simple Iterations:")

	res, count := equations.SimpleIterationsMethod(PHI, DPHI, 1.0, 2.0, 0.001)
	fmt.Println("answer: ", res)
	fmt.Println("count: ", count)

	fmt.Println("\nNewton:")

	res2, count2 := equations.NewtonMethod(F, DF, D2F, 0.001, 2)
	fmt.Println("answer: ", res2)
	fmt.Println("count: ", count2)
}


func SystemRunner() {
    fmt.Println("2.2-----Systems-----")
    PHI := []func([]float64) float64{
        func(x []float64) float64 {
            return math.Cos(x[1]) / 3
        },
        func(x []float64) float64 {
            return math.Pow(math.E, x[0]) / 3
        },
    }

    // Для системы: строим графики x = phi1(y) и y = phi2(x)
    var x1s, y1s, x2s, y2s []float64
    for t := 0.0; t <= 2.0; t += 0.01 {
        x1s = append(x1s, PHI[0]([]float64{0, t}))
        y1s = append(y1s, t)
        x2s = append(x2s, t)
        y2s = append(y2s, PHI[1]([]float64{t, 0}))
    }
    // Покажем выбранное начальное приближение (например, x0=0.5, y0=1)
    plotter.Plot2(x1s, y1s, x2s, y2s, "system_init", []string{"x=phi1(y)", "y=phi2(x)"})

    DPHI := [][]func([]float64) float64{
        {
            func(x []float64) float64 { return 0 },
            func(x []float64) float64 { return -math.Sin(x[1]) / 3 },
        },
        {
            func(x []float64) float64 { return math.Pow(math.E, x[0]) / 3 },
            func(x []float64) float64 { return 0 },
        },
    }
    F := []func([]float64) float64{
        func(x []float64) float64 { return 3*x[0] - math.Cos(x[1]) },
        func(x []float64) float64 { return 3*x[1] - math.Pow(math.E, x[0]) },
    }
    DF := [][]func([]float64) float64{
        {
            func(x []float64) float64 { return 3 },
            func(x []float64) float64 { return math.Sin(x[1]) },
        },
        {
            func(x []float64) float64 { return -math.Pow(math.E, x[0]) },
            func(x []float64) float64 { return 3 },
        },
    }

    fmt.Println("Simple Iterations:")
    res, count := systems.SimpleIterationsMethod(PHI, DPHI, 0.0, 0.5, 0.5, 1, 0.0001)
    for i := 0; i < 2; i++ {
        fmt.Printf("x%d = %f\n", i+1, res[i])
    }
    fmt.Println("count: ", count)

    fmt.Println("\nNewton:")
    res2, count2 := systems.NewtonMethod(F, DF, 0.0, 0.0, 0.0001)
    for i := 0; i < 2; i++ {
        fmt.Printf("x%d = %f\n", i+1, res2[i])
    }
    fmt.Println("count: ", count2)
}

func main() {
	fmt.Println("Select lab 2.X:")
	fmt.Println("1: equation")
	fmt.Println("2: system")

	var point int
	fmt.Scan(&point)
	switch point {
	case 1:
		EquationRunner()
	case 2:
		SystemRunner()
	default:
		fmt.Println("I don't know u wrong")
	}
}
