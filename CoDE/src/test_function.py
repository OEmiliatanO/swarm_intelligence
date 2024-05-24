# --- Test function for 112 midterm report of NSYSU swarm intelligence ---
# Editor: Cheng-Chun Chen

import numpy as np

def Ackley(x):
    x = np.array(x)
    temp1 = x * x
    temp2 = np.cos(2.0 * np.pi * x)
    return -20.0 * np.exp(-0.2 * np.sqrt(np.sum(temp1) / len(x))) - np.exp(np.sum(temp2) / len(x)) + 20.0 + np.exp(1)

def Rastrigin(x):
    x = np.array(x)
    temp1 = x * x - 10.0 * np.cos(2.0 * np.pi * x)
    return 10.0 * len(x) + np.sum(temp1)

def HappyCat(x):
    x = np.array(x)
    temp1 = x * x
    sum1 = np.sum(temp1)
    sum2 = np.sum(x)
    return np.power(np.abs(sum1 - len(x)), 0.25) + (sum1 * 0.5 + sum2) / len(x) + 0.5

def Rosenbrock(x):
    x = np.array(x)
    temp1 = (x[1:] - (x[:-1] * x[:-1])) * (x[1:] - (x[:-1] * x[:-1]))
    temp2 = (x - 1) * (x - 1)
    return np.sum(100.0 * temp1 + temp2[:-1])

def Zakharov(x):
    x = np.array(x)
    y = np.array([i + 1 for i in range(len(x))])
    temp1 = x * x
    temp2 = 0.5 * y * x
    return np.sum(temp1) + np.power(np.sum(temp2), 2) + np.power(np.sum(temp2), 4)

def Michalewicz(x):
    x = np.array(x)
    y = np.array([i + 1 for i in range(len(x))])
    temp1 = np.sin(x) * np.power(np.sin((y * x * x) / np.pi), 20)
    return -np.sum(temp1)

def Schwefel(x):
    x = np.array(x)
    temp1 = x * np.sin(np.sqrt(np.abs(x)))
    return 418.9829 * len(x) - np.sum(temp1)

def BentCigar(x):
    x = np.array(x)
    temp1 = x * x
    return temp1[0] + pow(10.0, 6) * np.sum(temp1[1:])

def DropWave(x):
    x = np.array(x)
    temp1 = np.dot(x, x)
    return 1.0 - ((1.0 + np.cos(12.0 * np.sqrt(temp1))) / (0.5 * temp1 + 2.0))

def Step(x):
    x = np.array([i + 0.5 for i in x])
    return np.sum(np.floor(x) * np.floor(x))

function_table = {1:Ackley, 2:Rastrigin, 3:HappyCat, 4:Rosenbrock, 5:Zakharov, 6:Michalewicz, 7:Schwefel, 8:BentCigar, 9:DropWave, 10:Step}
search_range = {1:[-32.768, 32.768], 2:[-5.12, 5.12], 3:[-20.0, 20.0], 4:[-10.0, 10.0], 5:[-10.0, 10.0], 6:[0.0, np.pi],7:[-500,500],8:[-100,100],9:[-5.12, 5.12],10:[-100,100]}


def cal_function_objective(x, func_num):
    return function_table[func_num](x)

def set_search_range(func_num):
    return search_range[func_num][0], search_range[func_num][1]
