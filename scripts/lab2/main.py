#!/usr/bin/env python3

from common import *
import matplotlib.pyplot as plt
import numpy as np
import random
from tqdm import tqdm

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
    results = []
    for m in STEEPEST_METHODS:
        x, f, data = read_data(A, b, c, Method.STEEPEST_DESCENT, steepest_method = m)
        # print(m)
        counts = []
        for (_, _, [iter_cnt]) in data:
            counts.append(int(iter_cnt))
        # results.append(sum(counts)/len(counts))
        results.append(len(data))
        # print('Avg:', sum(counts)/len(counts))
        # print('Med:', counts[len(counts)//2])
    return results

def draw_contours(A, b, c, method, level_step = 1, delta_step = 1, diag = False, initial = None):
    x, f, data = read_data(A, b, c, method, diag = diag, initial = initial)
    func = get_func(A, b, c)
    
    print(method)
    print('Iters:', len(data))
    print('x*:', x)
    print('f*:', f)
    level_i = level_step - 1
    delta_i = delta_step - 1
    # last = 100
    # skip = len(data) - last
    prev_x = [1]*len(b) if initial == None else initial
    levels = [func(*prev_x)]
    x_min = prev_x[0]
    x_max = prev_x[0]
    y_min = prev_x[1]
    y_max = prev_x[1]
    for (p, grad, other) in data:
        level_i = (level_i + 1) % level_step
        delta_i = (delta_i + 1) % delta_step

        if delta_i == 0:
            x_min = min(x_min, p[0])
            x_max = max(x_max, p[0])
            y_min = min(y_min, p[1])
            y_max = max(y_max, p[1])
            plt.arrow(prev_x[0], prev_x[1], p[0] - prev_x[0], p[1] - prev_x[1], head_width=0.02, width=0.002, color='red')
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
    for y in Y:
        z = []
        for x in X:
            z.append(func(x, y))
        Z.append(z)
    # print(levels)
    levels = set(levels)
    levels = list(levels)
    levels.sort()
    # print(levels)
    cntrs = plt.contour(X, Y, Z, levels = levels)
    fmt = {}
    for c in cntrs.levels:
        fmt[c] = '%.02f' % c
    plt.clabel(cntrs, cntrs.levels[::1], fmt=fmt)
    # plt.show()

def test(A, b, c, method):
    # gen_steepest(A, b, c)
    draw_contours(A, b, c, METHODS[0], level_step=1, grad_step=1)
    draw_contours(A, b, c, METHODS[1], level_step=2, grad_step=1)
    draw_contours(A, b, c, METHODS[2], level_step=1, grad_step=1)

def test_counts(n, k, method, N = 5):
    rs = []
    for i in range(N):
        diag = gen_diag(n, k)
        x, f, data_len = read_data(*diag, method, diag = True, no_data = True)
        rs.append(data_len)
    return max(rs)

def plot_counts(method, n, K = 200, K_STEP = 10):
    ks = range(1, K, K_STEP)
    counts = []
    for k in tqdm(ks):
        # print(f"k = {k}")
        counts.append(test_counts(n, k, method))

    plt.plot(ks, counts, label=('n = ' + str(n)));

def gen_diag(n, k, l = 1):
    L = k * l
    res = []
    res.append([l] + [0]*(n - 1))
    for i in range(1, n - 1):
        r = random.random() * (L - l) + l
        res.append([0]*i + [r] + [0]*(n - i - 1))
    res.append([0]*(n - 1) + [L])
    return (res, [0]*n, 0)


if __name__ == '__main__':
    # A = [[2, -1, 2],
    #      [-1, 1, -3],
    #      [2, -3, 11]]
    # b = [-100, 1, -30]
    # c = 10
    first = ([[100, -1],  # A
              [-1, 1]],
             [-10, 0], # b
             0)        # c

    second = ([[3, -1], # A
              [-1, 2]],
             [-5, 2], # b
             0)        # c

    third = ([[1, -1], # A
              [-1, 2]],
             [-10, 2], # b
             0)        # c
    third = ([[2, -1], # A
              [-1, 2]],
             [0, 0], # b
             0)        # c
    for k in range(1, 1000, 100):
        res = [0]*5
        for _ in range(3):
            func = gen_diag(3, k)
            results = steepest_iterations(*func)
            res = [a + b for (a, b) in zip(results, res)]
        res = [a / len(res) for a in res]
        res = [k] + res
        print(res)
    # draw_contours(*third, METHODS[1], level_step=1, delta_step=1, initial=[1, 1])
    # for i in range(1, 4):
    #     plot_counts(METHODS[2], 10**i, K = 1002, K_STEP=1)
    # plt.legend()
    plt.show()
    # test(*first)
    # test(*second)

    
    
