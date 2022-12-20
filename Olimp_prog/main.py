s = "01000100000100000100010001"
new_s = ""
for i in s:
    if(i == "1"):
        new_s += "0"
    
    else:
        new_s += "1"


print(new_s)