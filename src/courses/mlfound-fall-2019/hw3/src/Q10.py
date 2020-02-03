from math import exp
import numpy as np

def E_uv(u, v):
    return -2.0

def E_uu(u, v):
    return exp(u) + v * v * exp(u * v) + 2

def E_vv(u, v):
    return 4 * exp(2 * v) + 4

def E_u(u, v):
    return exp(u) + exp(u * v) * v + 2 * u - 2 * v - 3

def E_v(u, v):
    return 2 * exp(2 * v) - 2 * u + 4 * v - 2

def E(u, v):
    return exp(u) + exp(2 * v) + exp(u * v) + u * u - 2 * u * v + 2 * v * v - 3 * u - 2 * v

x = [0, 0]
for _ in range(5):
    print(E(x[0], x[1]))
    u = x[0]
    v = x[1]
    H = np.array([[E_uu(u, v), E_uv(u, v)], [E_uv(u, v), E_vv(u, v)]])
    G = np.array([[E_u(u, v),], [E_v(u, v),]])
    X = np.linalg.inv(H) @ G
    x[0] -= X[0][0]
    x[1] -= X[1][0]

print(E(x[0], x[1]))
