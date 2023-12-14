import numpy
import sympy as sm
import matplotlib.pyplot as ppt
from matplotlib.animation import FuncAnimation
import math

t = sm.Symbol('t')

x = sm.cos(6*t)
y = t + 0.2*sm.cos(3*t)

Vx = sm.diff(x)
Vy = sm.diff(y)

T = numpy.linspace(0, 10, 1000)

Xp = numpy.zeros_like(T)
Yp = numpy.zeros_like(T)
Vxp = numpy.zeros_like(T)
Vyp = numpy.zeros_like(T)

for i in numpy.arange(len(T)):
    Xp[i] = sm.Subs(x, t, T[i])
    Yp[i] = sm.Subs(y, t, T[i])
    Vxp[i] = sm.Subs(Vx, t, T[i])
    Vyp[i] = sm.Subs(Vy, t, T[i])

fig = ppt.figure()

graf = fig.add_subplot(1,1,1)
graf.axis('equal')
graf.set(xlim = [-2, 2], ylim = [-2, 2])

#graf.plot([-3, 0, -3], [2, 0, -2])

graf.plot(Xp, Yp)
Point = graf.plot(Xp[0], Yp[0], marker = 'o')[0]
Vplot = graf.plot([Xp[0], Xp[0]+Vxp[0]], [Yp[0], Yp[0]+Vyp[0]], 'r')[0]

def Rotation(Xt, Yt, Vxt, Vyt):
    Arx = numpy.array([-0.3, 0, -0.3])
    Ary = numpy.array([0.2, 0, -0.2])
    phi = math.atan2(Vyt, Vxt)

    RotX = Arx * numpy.cos(phi) - Ary * numpy.sin(phi)
    RotY = Arx * numpy.sin(phi) + Ary * numpy.cos(phi)

    RotX += Xt + Vxt
    RotY += Yt + Vyt
    return RotX, RotY

ArVx, ArVy = Rotation(Xp[0], Yp[0], Vxp[0], Vyp[0])
ArrowV = graf.plot(ArVx, ArVy, 'red')[0]

def animation(i):
    Point.set_data(Xp[i], Yp[i])
    Vplot.set_data([Xp[i], Xp[i]+Vxp[i]], [Yp[i], Yp[i]+Vyp[i]])
    ArVx, ArVy = Rotation(Xp[i], Yp[i], Vxp[i], Vyp[i])
    ArrowV.set_data(ArVx, ArVy)
    return

anim = FuncAnimation(fig, animation, frames = 1000, interval=1)

fig.show()
