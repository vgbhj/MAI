import os

for dname, dirs, files in os.walk("."):
    for fname in files:
        fpath = os.path.join(dname, fname)
        if(fname[:8] == "Makefile"):
            # Debug
            # print(fpath)
            with open(fpath) as f:
                s = f.read()
            s = s.replace("\\", "/")
            with open(fpath, "w") as f:
                f.write(s)