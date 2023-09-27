input()
input()
for i in map(int, input().split()):
    print((' '*(4-len(str(bin(i))[2:]))) + str(bin(i))[2:])

input()


for i in map(int, input().split()):
    print((' '*(4-len(str(bin(i))[2:]))) + str(bin(i))[2:])
