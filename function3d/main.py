import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import test_function as tfn
import sys

search_range = {1:[-32.768/5, 32.768/5], 2:[-5.12, 5.12], 3:[-20.0/2, 20.0/2], 4:[-10.0, 10.0], 5:[-10.0, 10.0], 6:[0.0, np.pi], 7:[-500,500], 8:[-100,100], 9:[-5.12, 5.12], 10:[-100,100]}
func_str_table = {1:"Ackley", 2:"Rastrigin", 3:"HappyCat", 4:"Rosenbrock", 5:"Zakharov", 6:"Michalewicz", 7:"Schwefel", 8:"BentCigar", 9:"DropWave", 10:"Step"}

fn = int(sys.argv[1])

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

ax.view_init(45, 20)

X = np.linspace(search_range[fn][0], search_range[fn][1], 256)
Y = np.linspace(search_range[fn][0], search_range[fn][1], 256)
X, Y = np.meshgrid(X, Y)

Z = []
for i in range(X.shape[0]):
    for j in range(X.shape[1]):
        Z.append(tfn.function_table[fn]([X[i, j],Y[i,j]]))
Z = np.array(Z)
Z = Z.reshape((X.shape[0], X.shape[1]))

plt.title(f"{func_str_table[fn]} function")
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

ax.plot_surface(X, Y, Z, rstride = 1, cstride = 1, cmap = 'jet')

ax.contour(X, Y, Z, offset = Z.min())

fig.savefig(f'{func_str_table[fn]}_3d.png')
plt.close()
