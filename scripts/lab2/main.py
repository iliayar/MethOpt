#!/usr/bin/env python3

from common import *
import matplotlib.pyplot as plt
import numpy as np
import random

def get_func(A, b, c):
    A = np.array(A)
    b = np.array(b)
    def f(*x):
        return 1/2 * A.dot(x).dot(x) + b.dot(x) + c
    return f

METHODS = [
    Method.STEEPEST_DESCENT,
    Method.GRADIENT_DESCENT,
    Method.CONJUGATE_GRADIENT
]

STEEPEST_METHODS = [
    SteepestMethod.DICHOTOMY,
    SteepestMethod.PARABOLAS,
    SteepestMethod.GOLDENSECTIONS,
    SteepestMethod.BRENT,
    SteepestMethod.FIBONACCI
]

def steepest_iterations(A, b, c):
    for m in STEEPEST_METHODS:
        x, f, data = read_data(A, b, c, Method.STEEPEST_DESCENT, steepest_method = m)
        print(m)
        counts = []
        for (_, _, [iter_cnt]) in data:
            counts.append(int(iter_cnt))
        print('Avg:', sum(counts)/len(counts))
        print('Med:', counts[len(counts)//2])

def draw_contours(A, b, c, method, level_step = 1, grad_step = 1, diag = False):
    x, f, data = read_data(A, b, c, method, diag = diag)
    func = get_func(A, b, c)
    
    print(method)
    print('Iters:', len(data))
    print('x*:', x)
    print('f*:', f)
    level_i = level_step - 1
    grad_i = grad_step - 1
    # last = 100
    # skip = len(data) - last
    prev_x = [1]*len(b)
    levels = [func(*prev_x)]
    x_min = 1
    x_max = 1
    y_min = 1
    y_max = 1
    for (p, grad, other) in data:
        level_i = (level_i + 1) % level_step
        grad_i = (grad_i + 1) % grad_step

        if grad_i == 0:
            x_min = min(x_min, p[0])
            x_max = max(x_max, p[0])
            y_min = min(y_min, p[1])
            y_max = max(y_max, p[1])
            plt.arrow(prev_x[0], prev_x[1], p[0] - prev_x[0], p[1] - prev_x[1], head_width=0.1, color='red')
            prev_x = p
        if level_i == 0:
            levels.append(func(*p))
    x_range = x_max - x_min
    x_min = x_min - x_range/3
    x_max = x_max + x_range/3
    y_range = y_max - y_min
    y_min = y_min - y_range/3
    y_max = y_max + y_range/3
    X = np.linspace(x_min, x_max, 200)
    Y = np.linspace(y_min, y_max, 200)
    Z = []
    for x in X:
        z = []
        for y in Y:
            z.append(func(x, y))
        Z.append(z)
    # print(levels)
    levels = set(levels)
    levels = list(levels)
    levels.sort()
    # print(levels)
    plt.contour(X, Y, Z, levels = levels)
    plt.show()

def test(A, b, c):
    # gen_steepest(A, b, c)
    draw_contours(A, b, c, METHODS[0], level_step=1, grad_step=1)
    draw_contours(A, b, c, METHODS[1], level_step=2, grad_step=1)
    draw_contours(A, b, c, METHODS[2], level_step=1, grad_step=1)

def test_counts(n, k, method, N = 10):
    s = 0
    for i in range(N):
        diag = gen_diag(n, k)
        x, f, data = read_data(*diag, method, diag = diag)
        s += len(data)
    return s / N

def plot_counts(method, n, K = 200, K_STEP = 10):
    ks = list(range(1, K, K_STEP))
    counts = []
    for k in ks:
        print(f"k = {k}")
        counts.append(test_counts(n, k, method))

    plt.plot(ks, counts);

def gen_diag(n, k, l = 1):
    L = k * l
    res = []
    res.append([l] + [0]*(n - 1))
    for i in range(1, n - 1):
        r = random.random() * (L - l) + l
        res.append([0]*i + [r] + [0]*(n - i - 1))
    res.append([0]*(n - 1) + [L])
    return (res, [1]*n, 0)

if __name__ == '__main__':
    # A = [[2, -1, 2],
    #      [-1, 1, -3],
    #      [2, -3, 11]]
    # b = [-100, 1, -30]
    # c = 10
    first = ([[2, -1],  # A
              [-1, 1]],
             [-10, 1], # b
             10)        # c

    second = ([[3, -1], # A
              [-1, 2]],
             [0, 1], # b
             0)        # c
    for i in range(1, 5):
        plot_counts(METHODS[0], 10**i, K = 2000, K_STEP=100)
    plt.show()
    # test(*first)
    # test(*second)

    
    
