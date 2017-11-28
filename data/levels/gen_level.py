#!/usr/bin/env python

from sys import argv, exit
import random as r
from itertools import product

if len(argv) < 2:
    print("usage:", argv[0], "n")
    exit(1)

n = int(argv[1])
rand_flag = bool(int(argv[2]))

# rand_samples = n//2;

all_1d = [i for i in range(n)]
all_i_j = product(all_1d, all_1d)

if rand_flag:
    for (i,j) in r.sample(list(all_i_j), n*n//2):
        print((float(i)+0.5)/n, (float(j)+0.5)/n, r.uniform(0.0,0.5), 1.0/n, 1.0/n, 0.3, 0.7, 0.5, 0.0)
else:
    for (i,j) in all_i_j:
        print((float(i)+0.5)/n, (float(j)+0.5)/n, 0.0, 1.0/n, 1.0/n, 0.3, 0.7, 0.5, 0.0)
