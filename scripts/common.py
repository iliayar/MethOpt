import json
import sys
import os
import subprocess
import csv
import numpy as np

PROJECT_ROOT = os.path.dirname(os.path.realpath((__file__))) + '/../'


class Method:
    pass
Method.DICHOTOMY      = 'dichotomy'
Method.PARABOLAS       = 'parabolas'
Method.GOLDENSECTIONS = 'goldensections'
Method.BRENT          = 'brent'
Method.FIBONACCI      = 'fibonacci'

def read_data(method = 'dichotomy', epsilon = 1e-5, interval = (-1, 1), polynom = []):
    cli = PROJECT_ROOT + 'build/lab1cli/lab1cli'
    if polynom != []:
        polynom = list(map(str, polynom))
    proc = subprocess.Popen(
        [cli] + polynom + [
            '-m', method,
            '-e', str(epsilon),
            '-l', str(interval[0]),
            '-r', str(interval[1]),
        ], stdout=subprocess.PIPE)

    for line in proc.stdout:
        yield json.loads(line)

def create_table(table, data):
    with open(PROJECT_ROOT + 'scripts/tables/' + table + '.csv', 'w') as csvfile:
        writer = csv.writer(csvfile)
        for row in data:
            writer.writerow(row)


def function(x):
    return x**4 - 1.5*np.arctan(x)
