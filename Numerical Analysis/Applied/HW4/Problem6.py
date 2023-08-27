import os
import numpy as np

from methods import Broyden, idx

alpha = 0.5
beta = 0.25


def f1(x: np.ndarray) -> float:
    return x[idx(1)] * (1 - x[idx(1)] - alpha * x[idx(2)] - beta * x[idx(3)])


def f2(x: np.ndarray) -> float:
    return x[idx(2)] * (1 - x[idx(2)] - beta * x[idx(1)] - alpha * x[idx(3)])


def f3(x: np.ndarray) -> float:
    return x[idx(3)] * (1 - x[idx(3)] - alpha * x[idx(1)] - beta * x[idx(2)])


def jacob(x: np.ndarray) -> np.ndarray:
    return np.array([
        [1 - 2 * x[idx(1)] - alpha * x[idx(2)] - beta *
         x[idx(3)], -alpha * x[idx(1)], -beta * x[idx(1)]],
        [-beta * x[idx(2)], 1 - 2 * x[idx(2)] - beta *
         x[idx(1)] - alpha * x[idx(3)],  -alpha * x[idx(2)]],
        [-alpha * x[idx(3)], -beta * x[idx(3)], 1 - 2 *
         x[idx(3)] - alpha * x[idx(1)] - beta * x[idx(2)]]
    ])


def main():
    os.system("cls")
    x0 = np.array([0.3, 0.5, 0.9], dtype=np.float64)
    funcs = np.array([f1, f2, f3])
    err = 10**(-5)

    result = Broyden(x0, funcs, jacob, err)
    print(f"Broyden method with x0 = {x0}:")
    print(result)


if __name__ == "__main__":
    main()
