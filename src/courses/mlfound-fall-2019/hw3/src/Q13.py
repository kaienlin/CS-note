import random
import numpy as np

N = 1000
T = 1000

def sign(x):
    return 1 if x > 0.0 else -1

def f(x1, x2):
    return sign(x1 ** 2 + x2 ** 2 - 0.6)

def gen_data():
    X = []
    Y = []
    for _ in range(N):
        X.append([1.0, ] + [np.random.uniform(-1.0, 1.0) for i in range(2)])
    
    for x in X:
        y = f(x[1], x[2])
        Y.append([y if random.random() >= 0.1 else -y, ])

    return np.array(X), np.array(Y)

def calc_ein(X, Y, W):
    P = X @ W
    err = 0
    for y, y_hat in zip(Y, P):
        if sign(y_hat[0]) != y[0]:
            err += 1
    return err / N

def main():
    avg = 0.0
    for t in range(T):
        X, Y = gen_data()
        W = np.linalg.pinv(X) @ Y
        E_in = calc_ein(X, Y, W)
        avg += E_in / T
    print(avg)

if __name__ == '__main__':
    main()
