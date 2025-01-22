import random
import sys

def generate_test(n, m, num_queries):
    grid = []
    for i in range(n):
        row = []
        for j in range(m):
            if random.random() < 0.2:
                row.append('#')
            else:
                row.append('.')
        grid.append(row)
    
    for _ in range(num_queries * 2):
        grid[random.randint(0, n - 1)][random.randint(0, m - 1)] = '.'
    
    queries = []
    for _ in range(num_queries):
        x1, y1 = random.randint(0, n - 1), random.randint(0, m - 1)
        x2, y2 = random.randint(0, n - 1), random.randint(0, m - 1)
        while grid[x1][y1] == '#' or grid[x2][y2] == '#':
            x1, y1 = random.randint(0, n - 1), random.randint(0, m - 1)
            x2, y2 = random.randint(0, n - 1), random.randint(0, m - 1)
        queries.append((x1 + 1, y1 + 1, x2 + 1, y2 + 1))

    return grid, queries

def save_to_file(filename, n, m, grid, queries):
    with open(filename, 'w') as f:
        f.write(f"{n} {m}\n")
        for row in grid:
            f.write("".join(row) + "\n")
        f.write(f"{len(queries)}\n")
        for x1, y1, x2, y2 in queries:
            f.write(f"{x1} {y1} {x2} {y2}\n")

def main():
    if len(sys.argv) < 2:
        print("Использование: python3 testgen.py <размер сетки>")
        return
    
    size = int(sys.argv[1])
    n, m = size, size
    num_queries = random.randint(1, 200)
    
    grid, queries = generate_test(n, m, num_queries)
    save_to_file("input.txt", n, m, grid, queries)
    print(f"Тестовые данные сохранены в файл input.txt")
    print(f"Размер сетки: {n} x {m}, Количество запросов: {len(queries)}")

if __name__ == "__main__":
    main()
