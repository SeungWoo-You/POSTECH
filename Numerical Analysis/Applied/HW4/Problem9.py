import os
import numpy as np

from methods import Continuation, Euler, idx


def f1(x: np.ndarray) -> float:
    return x[idx(1)]**2 - x[idx(2)]**2 + 2 * x[idx(2)]


def f2(x: np.ndarray) -> float:
    return 2 * x[idx(1)] + x[idx(2)]**2 - 6


def jacob(x: np.ndarray) -> np.ndarray:
    return np.array([
        [2 * x[idx(1)], -2 * x[idx(2)] + 2],
        [2, 2 * x[idx(2)]]
    ])


def main():
    os.system("cls")
    x0 = [
        np.array([0, 0], dtype=np.float64),
        np.array([1, 1], dtype=np.float64),
        np.array([3, -2], dtype=np.float64)
    ]
    funcs = np.array([f1, f2])
    N = 2

    for x_init in x0:
        result = Continuation(x_init, funcs, jacob, N, Euler)
        print(f"Continuation method with x0 = {x_init}:")
        print(result)
        print("")


if __name__ == "__main__":
    main()
