import os
import numpy as np

from methods import Newton, idx


def f1(x: np.ndarray) -> float:
    return 4 * x[idx(1)] - x[idx(2)] + x[idx(3)] - x[idx(1)] * x[idx(4)]


def f2(x: np.ndarray) -> float:
    return -x[idx(1)] + 3 * x[idx(2)] - 2 * x[idx(3)] - x[idx(2)] * x[idx(4)]


def f3(x: np.ndarray) -> float:
    return x[idx(1)] - 2 * x[idx(2)] + 3 * x[idx(3)] - x[idx(3)] * x[idx(4)]


def f4(x: np.ndarray) -> float:
    return x[idx(1)]**2 + x[idx(2)]**2 + x[idx(3)]**2 - 1


def jacob(x: np.ndarray) -> np.ndarray:
    return np.array([
        [-x[idx(4)] + 4, -1, 1, -x[idx(1)]],
        [-1, -x[idx(4)] + 3, -2, -x[idx(2)]],
        [1, -2, -x[idx(4)] + 3, -x[idx(3)]],
        [2 * x[idx(1)], 2 * x[idx(2)], 2 * x[idx(3)], 0]
    ])


def main():
    os.system("cls")
    x0 = [
        np.array([0.5, 7, -5, 5], dtype=np.float64),
        np.array([0.5, 0, -0.1, -0.1], dtype=np.float64),
        np.array([1, 1, 1, 1], dtype=np.float64)
    ]
    funcs = np.array([f1, f2, f3, f4])
    err = 10**(-5)

    for x_init in x0:
        result = Newton(x_init, funcs, jacob, err)
        print(f"Newton method with x0 = {x_init}:")
        print(result)
        print("")


if __name__ == "__main__":
    main()
