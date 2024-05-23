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
    N, D, k = [int(x) for x in f.readline().strip().split(' ')]
    pX = [[] for i in range(N)]
    gbest = []
    for it in range(k):
        for i in range(N):
            pX[i].append([float(x) for x in f.readline().strip().split(' ')])
        gbest.append(min([tfn.function_table[fn]([pX[i][it][0], pX[i][it][1]]) for i in range(N)]))

    for i in range(10):
        r = randint(0, 255)
        g = randint(0, 255)
        b = randint(0, 255)
        c = "#{:02x}{:02x}{:02x}".format(r, g, b)
        for it in range(k):
            plt.scatter(pX[i][it][0], pX[i][it][1], c=c, s=3, zorder=5)
            if it > 0:
                plt.plot([pX[i][it-1][0], pX[i][it][0]], [pX[i][it-1][1], pX[i][it][1]], c=c, linestyle='--', linewidth=1)

plt.title('DE population Path')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_jde_plot.png")
plt.savefig(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_jde_plot.png')
plt.close()

plt.figure()
plt.title('DE')
plt.xlabel('iteration')
plt.ylabel('value')
iteration = [i for i in range(k)]
plt.plot(iteration, gbest)
print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_jde_convergence.png")
plt.savefig(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_jde_convergence.png')
plt.close()

fig, ax = plt.subplots()
ax.contour(X, Y, Z)
ax.set_xlim(search_range[fn][0], search_range[fn][1])
ax.set_ylim(search_range[fn][0], search_range[fn][1])

points = []
for i in range(10):
    points.append(ax.scatter(pX[i][0][0], pX[i][0][1]))

def update_scatter(frame):
    it = frame
    ax.set_title('iter = ' + str(it))
    for i in range(10):
        points[i].set_offsets((pX[i][it][0], pX[i][it][1]))
    return tuple(points)

ani = animation.FuncAnimation(fig, update_scatter, frames=k, interval=200)
writer = animation.PillowWriter(fps=15,
                                metadata=dict(artist='Me'),
                                bitrate=1800)

print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_jde_path.gif")
ani.save(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_jde_path.gif', writer=writer)
