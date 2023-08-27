import os
import numpy as np

from methods import FPI, idx

alpha = 0.3
beta = 0.6


def g1(x: np.ndarray) -> float:
    return np.sqrt(x[idx(1)] * (1 - alpha * x[idx(2)] - beta * x[idx(3)]))


def g2(x: np.ndarray) -> float:
    return np.sqrt(x[idx(2)] * (1 - alpha * x[idx(3)] - beta * x[idx(1)]))


def g3(x: np.ndarray) -> float:
    return np.sqrt(x[idx(3)] * (1 - alpha * x[idx(1)] - beta * x[idx(2)]))


def main():
    os.system("cls")
    x0 = np.array([0.5, 0.5, 0.5], dtype=np.float64)
    funcs = np.array([g1, g2, g3])
    err = 10**(-5)

    result = FPI(x0, funcs, err)
    print(f"Fixed point iteration with x0 = {x0}:")
    print(result)
    print("")


if __name__ == "__main__":
    main()
