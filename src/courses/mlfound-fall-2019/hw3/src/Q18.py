import numpy as np

training_data_file_name = "hw3_train.dat"
test_data_file_name = "hw3_test.dat"

eta = 0.001
T = 2000
N = 0
dim = 20

def read_data(fname):
    X = []
    Y = []
    with open(fname, "r") as f:
        for ln in f.readlines():
            ln = ln.split()
            X.append([1.0,] + [ float(x) for x in ln[:-1] ])
            Y.append([int(ln[-1]),])
    global N
    N = len(X)
    return np.array(X), np.array(Y)

def theta(s):
    return 1.0 / (1.0 + np.exp(-s))

def train(X, Y):
    w = np.zeros(dim + 1)
    for _ in range(T):
        grad = np.zeros(dim + 1)
        for x, y in zip(X, Y):
            grad += -1.0 / N * theta(-y * np.dot(w, x)) * (y * x)
        w -= eta * grad
    return w

def calc_err(X, Y, W):
    P = X @ W
    err = 0
    for y, p in zip(Y, P):
        p = theta(p)
        yp = 1 if p > 0.5 else -1
        if yp != y:
            err += 1
    return err / N

def main():
    X, Y = read_data(training_data_file_name)
    W = train(X, Y)
    X, Y = read_data(test_data_file_name)
    E_out = calc_err(X, Y, W)
    print(E_out)

if __name__ == '__main__':
    main()