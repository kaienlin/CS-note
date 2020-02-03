import random
import numpy as np

N = 1000
T = 1000

G = [_ for _ in range(5)]

G[0] = np.array([[-1,], [-0.05,], [0.08,], [0.13,], [1.5,], [15.0,]])
G[1] = np.array([[-1,], [-0.05,], [0.08,], [0.13,], [1.5,], [1.5,]])
G[2] = np.array([[-1,], [-1.5,], [0.08,], [0.13,], [0.05,], [1.5,]])
G[3] = np.array([[-1,], [-1.5,], [0.08,], [0.13,], [0.05,], [0.05,]])
G[4] = np.array([[-1,], [-0.05,], [0.08,], [0.13,], [15.0,], [1.5,]])

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

def transform(X):
    Z = []
    for x in X:
        Z.append([1.0, x[1], x[2], x[1] * x[2], x[1] ** 2, x[2] ** 2])
    return np.array(Z)

def calc_ein(Z, Y, W):
    P = Z @ W
    err = 0
    for y, y_hat in zip(Y, P):
        if sign(y_hat[0]) != y[0]:
            err += 1
    return err / N

def main():
    X, Y = gen_data()
    Z = transform(X)
    W = np.linalg.pinv(Z) @ Y
    E_in = calc_ein(Z, Y, W)
    print(E_in)
    for i, g in enumerate(G):
        print(i, calc_ein(Z, Y, g))

if __name__ == '__main__':
    main()
