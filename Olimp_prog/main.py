L = []
for i in range(1, 250):
    L.append("+ " + "a"*i + " 1\n")
for i in range(1, 250):
    L.append("+ " + "b"*i + " 1\n")
for i in range(1, 250):
    L.append("+ " + "c"*i + " 1\n")
for i in range(1, 250):
    L.append("- " + "a"*i + "\n")
for i in range(1, 250):
    L.append("- " + "b"*i + "\n")
for i in range(1, 250):
    L.append("- " + "c"*i + "\n")

with open("/home/miha/Documents/MAI/DA/lab2-3/solution/input2.txt", "w") as file1:
    file1.writelines(L)