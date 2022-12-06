def f(a,arr):
    ans = set()
    s1 = str(a)
    for _ in arr:
        s2 = str(_)
        for i in s1:
            if(i in s2):
                ans.add(s2)


    return sorted(ans)          


n = 100
arr = list(range(1, n+1))

for i in range(1, n+1):
    print(i, len(f(i, arr)), " = ", end='') 
    # print(*f(i, arr))
    print()