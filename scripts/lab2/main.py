#!/usr/bin/env python3

from common import *
import matplotlib.pyplot as plt
import numpy as np

def get_func(A, b, c):
    A = np.array(A)
    b = np.array(b)
    def f(x):
        return 1/2 * A.dot(x).dot(x) + b.dot(x) + c
    return f

METHODS = [
    Method.STEEPEST_DESCENT,
    Method.GRADIENT_DESCENT,
    Method.CONJUGATE_GRADIENT
]


if __name__ == '__main__':
    # A = [[2, -1, 2],
    #      [-1, 1, -3],
    #      [2, -3, 11]]
    # b = [-100, 1, -30]
    # c = 10
    A = [[2, -1],
         [-1, 1]]
    b = [-100, 1]
    c = 10
    x, f, data = read_data(A, b, c, METHODS[2])
    func = get_func(A, b, c)
    # print(x, f, data)
    
    print(len(data))
    STEP = 1
    i = STEP - 1
    # last = 100
    # skip = len(data) - last
    levels = []
    for (p, grad) in data:
        plt.arrow(p[0], p[1], -grad[0], -grad[1], head_width=0.4, width=0.1, color='red')
        i = (i + 1) % STEP
        # skip -= 1
        # if skip > 0:
        #     continue
        if i != 0:
            continue
        levels.append(func(p))
    levels.sort()
    X = np.linspace(0, 200, 100)
    Y = np.linspace(0, 200, 100)
    Z = []
    for x in X:
        z = []
        for y in Y:
            z.append(func((x, y)))
        Z.append(z)
    plt.contour(X, Y, Z, levels = levels)
    plt.show()
    
    
