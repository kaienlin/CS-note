import numpy as np
import matplotlib.pyplot as plt
import random, math
import requests

def sign(x):
    return 1 if x > 0 else -1

def stump_decision(D, N, dim):
    D = sorted(D, key=lambda d: d[0][dim])

    err = 0
    for d in D:
        # theta = 0 => all y is 1
        err += 1 if d[1] != 1 else 0

    err2 = N - err
    
    best = [1, 0, err]

    for theta in range(1, N):
        # y = 0 if index < theta else 1
        # only y[theta-1] changes: 1 -> 0
        err += 1 if D[theta - 1][1] == 1 else -1 
        
        if err < best[2]:
            best[0] = 1
            best[1] = (D[theta][0][dim] + D[theta - 1][0][dim]) / 2 if theta != 0 else (D[theta][0][dim] + -1) / 2
            best[2] = err

    # s = -1
    for theta in range(0, N):
        # y = 1 if index < theta else 0
        err2 += 1 if D[theta - 1][1] == -1 else -1
        
        if err2 < best[2]:
            best[0] = -1
            best[1] = (D[theta][0][dim] + D[theta - 1][0][dim]) / 2 if theta != 0 else (D[theta][0][dim] + -1) / 2
            best[2] = err2

    return tuple(best)


def get_data(url):
    raw = requests.get(url).text
    D = []
    for ln in raw.strip().split("\n"):
        ln = ln.strip().split(" ")
        D.append((np.array([float(x) for x in ln[:-1]]), int(ln[-1])))

    return D


def multi_dim_stump(D):
    max_dim = len(D[0][0])
    N = len(D)
    best = (0, 0, 1<<61)
    dim = -1
    for d in range(max_dim):
        g = stump_decision(D, N, d)
        if g[2] < best[2]:
            best = g
            dim = d
    
    return (best[0], best[1], dim, best[2])


def calc_Eout(D, s, theta, dim):
    err = 0
    for d in D:
        if s * sign(d[0][dim] - theta) != d[1]:
            err += 1
    return err / len(D)


if __name__ == "__main__":
    url_train = 'https://www.csie.ntu.edu.tw/~htlin/mooc/datasets/mlfound_math/hw2_train.dat'
    D_train = get_data(url_train)

    g = multi_dim_stump(D_train)
    print(f"Selected hypothesis is: s = {g[0]}, theta = {g[1]}, dim = {g[2]}, E_in = {g[3] / len(D_train)}")

    url_test = 'https://www.csie.ntu.edu.tw/~htlin/mooc/datasets/mlfound_math/hw2_test.dat'
    D_test = get_data(url_test)
    E_out = calc_Eout(D_test, g[0], g[1], g[2])
    print(f"E_out = {E_out}")
