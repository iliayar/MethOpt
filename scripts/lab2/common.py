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

class SteepestMethod:
    pass
SteepestMethod.DICHOTOMY      = 'dichotomy'
SteepestMethod.PARABOLAS      = 'parabolas'
SteepestMethod.GOLDENSECTIONS = 'goldensections'
SteepestMethod.BRENT          = 'brent'
SteepestMethod.FIBONACCI      = 'fibonacci'

def read_data(A, b, c, method = Method.CONJUGATE_GRADIENT, initial = None, epsilon = 1e-4, steepest_method = SteepestMethod.BRENT, diag = False):
    with open('./input.txt', 'w') as f:
        if type(A[0]) == list:
            for i in A:
                for j in i:
                    f.write(str(j) + ' ')
                f.write('\n')
        else:
            for i in A:
                f.write(str(i[i]) + ' ')
            f.write('\n')
        for i in b:
            f.write(str(i) + ' ')
        f.write('\n')
        f.write(str(c))
        f.write('\n')
    cli = PROJECT_ROOT + 'build/lab2cli/lab2cli'
    dim = len(b)
    if initial == None:
        initial = []
    else:
        initial = ['-i', ' '.join(initial)]
    if diag:
        diag = ['--diag']
    else:
        diag = []
    proc = subprocess.Popen(
        [cli,
         '-m', method,
         '-d', str(dim),
         '-f', './input.txt',
         '-e', str(epsilon),
         '--smethod', steepest_method
        ] + initial + diag, stdout=subprocess.PIPE)
    x = list(map(float, proc.stdout.readline().decode().split()))
    f = float(proc.stdout.readline().decode())

    data = []
    n = int(proc.stdout.readline().decode())
    for i in range(n):
        x = list(map(float, proc.stdout.readline().decode().split()))
        grad = list(map(float, proc.stdout.readline().decode().split()))
        other = []
        while True:
            has_other = int(proc.stdout.readline().decode())
            if has_other == 1:
                other.append(proc.stdout.readline().decode())
            else:
                break
        data += [(x, grad, other)]
    return (x, f, data)

def create_table(table, data):
    with open(PROJECT_ROOT + 'scripts/tables/' + table + '.csv', 'w') as csvfile:
        writer = csv.writer(csvfile)
        for row in data:
            writer.writerow(row)


def function(x):
    return x**4 - 1.5*np.arctan(x)
