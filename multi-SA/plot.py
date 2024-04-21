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
    S = [[] for i in range(N)]
    gbest = []
    for it in range(k):
        for n in range(N):
            S[n].append([float(x) for x in f.readline().strip().split(' ')])
        gbest.append([float(x) for x in f.readline().strip().split(' ')])

    for n in range(min(N, 10)):
        r, g, b = randint(0, 255), randint(0, 255), randint(0, 255)
        c = "#{:02x}{:02x}{:02x}".format(r, g, b)
        for it in range(k):
            plt.scatter(S[n][it][0], S[n][it][1], c=c, s=3, zorder=5)
            if it > 0:
                plt.plot([S[n][it-1][0], S[n][it][0]], [S[n][it-1][1], S[n][it][1]], c=c, linestyle='--', linewidth=1)

plt.title('multi-SA Path')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_plot.png")
plt.savefig(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_plot.png')
plt.close()

plt.figure()
plt.title('multi-SA')
plt.xlabel('iteration')
plt.ylabel('value')
iteration = [i for i in range(k)]
value = [min([tfn.function_table[fn]([S[i][it][0], S[i][it][1]]) for i in range(N)]) for it in range(k)]
plt.plot(iteration, value)
print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_convergence.png")
plt.savefig(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_convergence.png')
plt.close()

fig, ax = plt.subplots()
ax.contour(X, Y, Z)
ax.set_xlim(search_range[fn][0], search_range[fn][1])
ax.set_ylim(search_range[fn][0], search_range[fn][1])

points = []
for i in range(min(N, 10)):
    points.append(ax.scatter(S[i][0][0], S[i][0][1]))

def update_scatter(frame):
    it = frame
    ax.set_title('iter = ' + str(it))
    for i in range(min(N, 10)):
        points[i].set_offsets((S[i][it][0], S[i][it][1]))
    return tuple(points)

ani = animation.FuncAnimation(fig, update_scatter, frames=k, interval=200)
writer = animation.PillowWriter(fps=15,
                                metadata=dict(artist='Me'),
                                bitrate=1800)

print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_path.gif")
ani.save(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_path.gif', writer=writer)
