#!/usr/bin/env python

import os
import matplotlib.pyplot as plt
import numpy as np
from common import PROJECT_ROOT, function

if __name__ == '__main__':
    x = np.linspace(-5, 5, 100)
    y = function(x)

    plt.gca().set_ylim([-1, 5])
    plt.plot(x, y)
    plt.savefig(PROJECT_ROOT + 'scripts/images/function.png')
    
