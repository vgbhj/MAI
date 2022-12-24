import matplotlib.pyplot as plt
import numpy as np

f = open("flight.log", "r")
twr_list = []
time = []
speed = []
mass = []
for i in f.readlines():
    if(i.find("TWR") != -1):
        twr_list.append(float(i[i.find("TWR")+5:i.find("TWR")+8]))
    if(i.find("Time") != -1):
        time.append(int(i[i.find(("h"))-2:i.find(("h"))])*60+int(i[i.rfind(("m"))-2:i.rfind(("m"))])%60)
        # print(int(i[i.find(("h"))-2:i.find(("h"))])*60+int(i[i.rfind(("m"))-2:i.rfind(("m"))])%60)

f.close()

theoretical_twr = []
plt.plot(range(len(twr_list)), [twr for twr in twr_list], color="red", label="KSP")
plt.xticks(np.arange(0, len(twr_list), 200))
plt.yticks(np.arange(0, 3, 0.2))
plt.grid(True)

plt.show()