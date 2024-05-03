import numpy as np
from random import randint, random
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from src import test_function as tfn
import sys

func_str_table = {1:"Ackley", 2:"Rastrigin", 3:"HappyCat", 4:"Rosenbrock", 5:"Zakharov", 6:"Michalewicz"}
search_range = {1:[-32.768, 32.768], 2:[-5.12, 5.12], 3:[-20.0, 20.0], 4:[-10.0, 10.0], 5:[-10.0, 10.0], 6:[0.0, np.pi]}

fn = int(sys.argv[1])
x = np.linspace(search_range[fn][0], search_range[fn][1], 100)
y = np.linspace(search_range[fn][0], search_range[fn][1], 100)
X, Y = np.meshgrid(x, y)
Z = []
for i in range(X.shape[0]):
    for j in range(X.shape[1]):
        Z.append(tfn.function_table[fn]([X[i, j],Y[i,j]]))
Z = np.array(Z)
Z = Z.reshape((X.shape[0], X.shape[1]))

plt.figure()
contour = plt.contour(X, Y, Z)
#plt.clabel(contour, inline = True)
plt.colorbar(contour, label=f'{func_str_table[fn]} Function Value')

with open(f"plot/{func_str_table[fn]}/{func_str_table[fn]}_path.txt") as f:
    pX = []
    step = []
    gbest = []
    while True:
        k = f.readline()
        if k is None or k == '':
            break
        k = int(k.strip())
        pX.append([])
        step.append([])
        for j in range(k):
            pX[-1].append([float(x.strip()) for x in f.readline().strip().split(' ')])
            step[-1].append([float(x.strip())/2 for x in f.readline().strip().split(' ')])
        gbest.append(float(f.readline().strip()))

plt.figure()
plt.title('DE')
plt.xlabel('iteration')
plt.ylabel('value')
iteration = [i for i in range(len(pX))]
plt.plot(iteration, gbest)
print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_de_convergence.png")
plt.savefig(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_de_convergence.png')
plt.close()

fig, ax = plt.subplots()
ax.contour(X, Y, Z)
ax.set_xlim(search_range[fn][0], search_range[fn][1])
ax.set_ylim(search_range[fn][0], search_range[fn][1])

points = []
borders = []

dirr = [[-1,-1,-1,+1], [-1,+1,+1,+1], [+1,+1,+1,-1], [+1,-1,-1,-1]]

for i in range(len(pX[0])):
    points.append(ax.scatter(pX[0][i][0], pX[0][i][1]))
    cx, cy = pX[0][i][0], pX[0][i][1]
    for j in range(4):
        a, b = pX[0][i][0] + dirr[j][0] * step[0][i][0], pX[0][i][1] + dirr[j][1] * step[0][i][1]
        c, d = pX[0][i][0] + dirr[j][2] * step[0][i][0], pX[0][i][1] + dirr[j][3] * step[0][i][1]
        line, = ax.plot([a, c], [b, d], c="red", linewidth=1)
        borders.append(line)

def update_scatter(frame):
    it = frame
    ax.set_title('iter = ' + str(it))
    for i in range(len(pX[it])):
        points[i].set_offsets((pX[it][i][0], pX[it][i][1]))
        for j in range(4):
            a, b = pX[it][i][0] + dirr[j][0] * step[it][i][0], pX[it][i][1] + dirr[j][1] * step[it][i][1]
            c, d = pX[it][i][0] + dirr[j][2] * step[it][i][0], pX[it][i][1] + dirr[j][3] * step[it][i][1]
            borders[i*4+j].set_xdata([a, c])
            borders[i*4+j].set_ydata([b, d])
            
    return tuple(points)

ani = animation.FuncAnimation(fig, update_scatter, frames=len(pX), interval=1000)
writer = animation.PillowWriter(fps=1,
                                metadata=dict(artist='Me'),
                                bitrate=1800)

print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_de_path.gif")
ani.save(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_de_path.gif', writer=writer)
