import numpy as np
from random import randint, random
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from src import test_function as tfn
import sys

search_range = {1:[-32.768, 32.768], 2:[-5.12, 5.12], 3:[-20.0, 20.0], 4:[-10.0, 10.0], 5:[-10.0, 10.0], 6:[0.0, np.pi],7:[-500,500],8:[-100,100],9:[-5.12, 5.12],10:[-100,100]}
func_str_table = {1:"Ackley", 2:"Rastrigin", 3:"HappyCat", 4:"Rosenbrock", 5:"Zakharov", 6:"Michalewicz", 7:"Schwefel", 8:"BentCigar", 9:"DropWave", 10:"Step"}

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
    D, k = [int(x) for x in f.readline().strip().split(' ')]
    S = []
    gbest = []
    for it in range(k):
        S.append([float(x) for x in f.readline().strip().split(' ')])
        gbest.append([float(x) for x in f.readline().strip().split(' ')])

    r = 255
    g = 0#randint(0, 255)
    b = 0#randint(0, 255)
    c = "#{:02x}{:02x}{:02x}".format(r, g, b)
    nowp_s = nowp = gbest[0]
    gbest_pruned = []
    S_pruned = []
    gbest_pruned.append(nowp)
    S_pruned.append(nowp_s)
    plt.scatter(nowp[0], nowp[1], c='red', s=3, zorder=5)
    for it in range(k):
        if abs(nowp[0] - gbest[it][0]) > 1e-3 or abs(nowp[1] - gbest[it][1]) > 1e-3:
            #plt.scatter(gbest[it][0], gbest[it][1], c='red', s=3, zorder=5)
            #plt.plot([nowp[0], gbest[it][0]], [nowp[1], gbest[it][1]], c=c, linestyle='--', linewidth=1)
            plt.scatter(S[it][0], S[it][1], c='red', s=3, zorder=5)
            plt.plot([nowp_s[0], S[it][0]], [nowp_s[1], S[it][1]], c=c, linestyle='--', linewidth=1)
            nowp = gbest[it]
            nowp_s = S[it]
            gbest_pruned.append(nowp)
            S_pruned.append(nowp_s)

plt.title('SA Path')
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_plot.png")
plt.savefig(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_plot.png')
plt.close()

plt.figure()
plt.title('SA')
plt.xlabel('iteration')
plt.ylabel('value')
iteration = [i for i in range(len(S))]
value = [tfn.function_table[fn]([S[i][0], S[i][1]]) for i in range(len(S))]
gfbest = [tfn.function_table[fn]([gbest[i][0], gbest[i][1]]) for i in range(len(S))]
plt.plot(iteration, value, label='S')
plt.plot(iteration, gfbest, label='global best')
plt.legend()
print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_convergence.png")
plt.savefig(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_convergence.png')
plt.close()

fig, ax = plt.subplots()
ax.contour(X, Y, Z)
ax.set_xlim(search_range[fn][0], search_range[fn][1])
ax.set_ylim(search_range[fn][0], search_range[fn][1])

points = []
#points.append(ax.scatter(gbest_pruned[0][0], gbest_pruned[0][1]))
points.append(ax.scatter(S_pruned[0][0], S_pruned[0][1]))

def update_scatter(frame):
    it = frame
    ax.set_title('iter = ' + str(it))
    for i in range(len(points)):
        #points[i].set_offsets((gbest_pruned[it][0], gbest_pruned[it][1]))
        points[i].set_offsets((S_pruned[it][0], S_pruned[it][1]))
    return tuple(points)

ani = animation.FuncAnimation(fig, update_scatter, frames=len(S_pruned), interval=200)
writer = animation.PillowWriter(fps=15,
                                metadata=dict(artist='Me'),
                                bitrate=1800)

print(f"save to plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_path.gif")
ani.save(f'plot/{func_str_table[fn]}/{func_str_table[fn]}_sa_path.gif', writer=writer)
