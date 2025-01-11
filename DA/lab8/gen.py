import random

def generate_test_file(filename, num_tests, max_length):
    with open(filename, 'w') as f:
        for _ in range(num_tests):
            n = max_length
            sequence = [random.choice([1, 2, 3]) for _ in range(n)]
            f.write(f"{n}\n")
            f.write(" ".join(map(str, sequence)) + "\n")

num_tests = 1
max_length = 10

generate_test_file("test.txt", num_tests, max_length)
