import numpy as np
from random import randint, random
import matplotlib.pyplot as plt
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

with open(f"{func_str_table[fn]}_path.txt") as f:
    N, D, k = [int(x) for x in f.readline().strip().split(' ')]
    X = [[] for i in range(N)]
    V = [[] for i in range(N)]
    for it in range(k):
        for i in range(N):
            X[i].append([float(x) for x in f.readline().strip().split(' ')])
            V[i].append([float(v) for v in f.readline().strip().split(' ')])

    for i in range(10):
        r = randint(0, 255)
        g = randint(0, 255)
        b = randint(0, 255)
        c = "#{:02x}{:02x}{:02x}".format(r, g, b)
        for it in range(k):
            plt.scatter(X[i][it][0], X[i][it][1], c=c, s=3, zorder=5)
            if it > 0:
                plt.plot([X[i][it-1][0], X[i][it][0]], [X[i][it-1][1], X[i][it][1]], c=c, linestyle='--', linewidth=1)

plt.title('PSO Particle Path')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
plt.savefig(f'{func_str_table[fn]}_pso_plot.png')
