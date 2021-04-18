import sys
import os
import subprocess
import csv
import numpy as np
import io

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

def read_data(A, b, c, method = Method.CONJUGATE_GRADIENT, initial = None, epsilon = 1e-4, steepest_method = SteepestMethod.BRENT, diag = False, no_data = False):
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
    if no_data:
        no_data = ['--no-data']
    else:
        no_data = []
    proc = subprocess.Popen(
        [cli,
         '-m', method,
         '-d', str(dim),
         '-f', '-',
         '-e', str(epsilon),
         '--smethod', steepest_method
         ] + initial + diag + no_data, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    inp = []
    if not diag:
        for i in A:
            inp.append(' '.join(map(str, i)))
    else:
        inp.append(' '.join([str(A[i][i]) for i in range(len(b))]))
    inp.append(' '.join(map(str, b)))
    inp.append(str(c))

    out_raw = proc.communicate('\n'.join(inp).encode())[0].decode()
    out = io.StringIO(out_raw)

    x = list(map(float, out.readline().split()))
    f = float(out.readline())

    data = []
    n = int(out.readline())
    if len(no_data) == 1:
        return (x, f, n)
    for i in range(n):
        x = list(map(float, out.readline().split()))
        grad = list(map(float, out.readline().split()))
        other = []
        while True:
            has_other = int(out.readline())
            if has_other == 1:
                other.append(out.readline())
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
