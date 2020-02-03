import numpy as np
import matplotlib.pyplot as plt
import random, math

N = 2000

def sign(x):
    return 1 if x > 0 else -1

def gen_x(lb=-1, ub=1):
    return np.random.uniform(lb, ub, size=N)

def gen_y(X):
    Y = []
    for x in X:
        y = sign(x)
        Y.append(-y if random.random() <= 0.2 else y)
    return Y

def stump_decision(D):
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
            best[1] = (D[theta][0] + D[theta - 1][0]) / 2 if theta != 0 else (D[theta][0] + -1) / 2
            best[2] = err

    # s = -1
    for theta in range(0, N):
        # y = 1 if index < theta else 0
        err2 += 1 if D[theta - 1][1] == -1 else -1
        
        if err2 < best[2]:
            best[0] = -1
            best[1] = (D[theta][0] + D[theta - 1][0]) / 2 if theta != 0 else (D[theta][0] + -1) / 2
            best[2] = err2

    return tuple(best)

if __name__ == "__main__":
    T = 1000
    Ein_avg = 0.0
    Eout_avg = 0.0
    diff_avg = 0.0
    
    diff_list = []
    for _ in range(T):
        X = gen_x()
        X.sort()
        Y = gen_y(X)
        D = [ (x, y) for x, y in zip(X, Y) ]
        g = stump_decision(D)
        s, theta, E_in = g[0], g[1], (g[2] / N)
        E_out = 0.5 + 0.3 * s * (np.abs(theta) - 1)

        Ein_avg += E_in
        Eout_avg += E_out
        diff_avg += E_in - E_out

        diff_list.append(E_in - E_out)

    fig, ax = plt.subplots()
    num_bins = 100
    n, bins, patches = ax.hist(diff_list, num_bins, facecolor='lightskyblue', alpha=0.5)

    if N == 20: ax.set_title(f"Figure 1: 1-D decision stump with small N")
    else: ax.set_title(f"Figure 2: 1-D decision stump with big N")

    ax.set_xlabel(r"$E_{in} - E_{out}$")
    ax.set_ylabel(r"frequency")
    fig.tight_layout()
    plt.grid(True)
    plt.show()

    print(f"Average E_in = {Ein_avg / T}")
    print(f"Average E_out = {Eout_avg / T}")
    print(f"Average E_in - E_out = {diff_avg / T}")
