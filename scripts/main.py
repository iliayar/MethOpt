#!/usr/bin/env python

from common import read_data, PROJECT_ROOT, create_table, Method

import sys
import csv
import numpy as np
import matplotlib.pyplot as plt

methods = [
    Method.DICHOTOMY,
    Method.GOLDENSECTIONS,
    Method.FIBONACCI,
    Method.PARABOLAS,
    Method.BRENT,
]

def gen_table(method):
    rows = []
    for data in read_data(method = method):
        if(data['type'] == 'minimum'):
            break;
        data = data['data']
        row = []
        for field in data:
            if field['type'] == 'point':
                row += [float(field['x']), float(field['y'])]
            elif field['type'] == 'interval':
                row += [float(field['left']), float(field['right'])]
            elif field['type'] == 'function':
                pass
        rows += [row]
    create_table(method, rows)

def count_calls(method = Method.DICHOTOMY, epsilon = 1e-5):
   for data in read_data(epsilon = epsilon, method = method):
       if(data['type'] == 'minimum'):
           return int(data['call_count'])
def gen_count_graph(method):
    epsilons = np.linspace(-1, -20, 500)
    counts = [count_calls(epsilon = eps, method = method) for eps in np.exp(epsilons)]
    plt.plot(epsilons, counts, label = method);
    plt.legend()
    plt.xlabel('$\\log \\varepsilon$')
    plt.ylabel("Call count")
    plt.savefig(PROJECT_ROOT + 'scripts/images/count_' + method + '.png')

def gen_gprahs():
    for method in methods:
        print("Generating graph for " + method + " method")
        gen_count_graph(method)
        plt.clf()
def gen_tables():
    for method in methods:
        print("Generating table for " + method + " method")
        gen_table(method)

def test_polynom(polynom, x_range, plt = plt):
    x = np.linspace(x_range[0], x_range[1], (x_range[1] - x_range[0])*100)
    y = 0
    for p in polynom[::-1]:
        y *= x
        y += p
    plt.plot(x, y, label = '$' + ' + '.join(str(p) + '\\cdot x^' + str(i) for (i, p) in enumerate(polynom)) + '$');
    for method in methods:
        for data  in read_data(method = method, interval = x_range, polynom = polynom):
            if(data['type'] == 'minimum'):
                print(method, data['x'], data['y'])
                plt.plot(data['x'], data['y'], 'o')
    plt.legend()

if __name__ == "__main__":
    # gen_gprahs()
    # gen_tables()
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)
    test_polynom([0, 0, 1], (-1, 1), plt = ax1)
    test_polynom([0, -0.4, 2, -0.2], (-5, 12), plt = ax2)
    test_polynom([-0.1, -0.9, -1.4, 0.3, 0.3], (-3, 3), plt = ax3)
    test_polynom([-0.1, 0.1, -0.5, 0.1, -0.8, 0.1], (-5, 5), plt = ax4)
    handles, labels = ax4.get_legend_handles_labels()
    # plt.gcf().legend(handles, labels, loc = 'upper center')
    plt.gcf().set_size_inches(10, 10)
    plt.savefig(PROJECT_ROOT + 'scripts/images/polynom.png', dpi=100)
    plt.show()

