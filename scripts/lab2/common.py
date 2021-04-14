import sys
import os
import subprocess
import csv
import numpy as np

PROJECT_ROOT = os.path.dirname(os.path.realpath((__file__))) + '/../../'


class Method:
    pass
Method.GRADIENT_DESCENT   = 'gradient'
Method.STEEPEST_DESCENT   = 'steepest'
Method.CONJUGATE_GRADIENT = 'conjugate'

def read_data(A, b, c, method = Method.CONJUGATE_GRADIENT):
    with open('./input.txt', 'w') as f:
        for i in A:
            for j in i:
                f.write(str(j) + ' ')
            f.write('\n')
        for i in b:
            f.write(str(i) + ' ')
        f.write('\n')
        f.write(str(c))
        f.write('\n')
    cli = PROJECT_ROOT + 'build/lab2cli/lab2cli'
    dim = len(b)
    proc = subprocess.Popen(
        [cli,
         '-m', method,
         '-d', str(dim),
         '-f', './input.txt'
        ], stdout=subprocess.PIPE)
    x = list(map(float, proc.stdout.readline().decode().split()))
    f = float(proc.stdout.readline().decode())

    data = []
    n = int(proc.stdout.readline().decode())
    for i in range(n):
        x = list(map(float, proc.stdout.readline().decode().split()))
        grad = list(map(float, proc.stdout.readline().decode().split()))
        data += [(x, grad)]
    return (x, f, data)

def create_table(table, data):
    with open(PROJECT_ROOT + 'scripts/tables/' + table + '.csv', 'w') as csvfile:
        writer = csv.writer(csvfile)
        for row in data:
            writer.writerow(row)


def function(x):
    return x**4 - 1.5*np.arctan(x)
