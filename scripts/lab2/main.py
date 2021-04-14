#!/usr/bin/env python3

from common import *
import matplotlib.pyplot as plt
import numpy as np

def get_func(A, b, c):
    A = np.array(A)
    b = np.array(b)
    def f(x):
        return 1/2 * (A * x).dot(x) + b.dot(x) + c
    return f

if __name__ == '__main__':
    A = [[2, -1, 2],
         [-1, 1, -3],
         [2, -3, 11]]
    b = [-100, 1, -30]
    c =10
    x, f, data = read_data(A, b, c, Method.GRADIENT_DESCENT)
    func = get_func(A, b, c)
    # print(x, f, data)
    STEP = 200
    i = STEP - 1
    for d in data:
        i = (i + 1) % STEP
        if i != 0:
            continue
        p, grad = d
        Z = np.linspace(p[2] - 100, p[2] + 100, 100)
        F = [func((p[0], p[1], z)) for z in Z]
        plt.plot(Z, F, color='blue')
        # f = func(p)
        # plt.plot(p, [f, f, f], marker='.')
        # plt.scatter([x[0]], [x[1]])
        plt.arrow(p[0], p[1], grad[0], grad[1], head_width=0.2, width=0.05)
    plt.show()
    
    
