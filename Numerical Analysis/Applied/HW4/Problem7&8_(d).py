import os
import numpy as np

from methods import Steepest_Descent, Newton, idx


def f1(x: np.ndarray) -> float:
    return np.sin(4 * np.pi * x[idx(1)] * x[idx(2)]) - 2 * x[idx(2)] - x[idx(1)]


def f2(x: np.ndarray) -> float:
    return (4 * np.pi - 1) / (4 * np.pi) * (np.exp(2 * x[idx(1)]) - np.e) + 4 * np.e * x[idx(2)]**2 - 2 * np.e * x[idx(1)]


def jacob(x: np.ndarray) -> np.ndarray:
    return np.array([
        [np.cos(4 * np.pi * x[idx(1)] * x[idx(2)]) * 4 * np.pi * x[idx(2)] - 1,
         np.cos(4 * np.pi * x[idx(1)] * x[idx(2)]) * 4 * np.pi * x[idx(1)] - 2],
        [(4 * np.pi - 1) / (2 * np.pi) * np.exp(2 * x[idx(1)]) - 2 * np.e,
         8 * np.e * x[idx(2)]]
    ])


def main():
    os.system("cls")
    x0 = np.array([0, 0.1], dtype=np.float64)
    funcs = np.array([f1, f2])
    TOL = [0.05, 10**(-6)]
    methods = [Steepest_Descent, Newton]
    i = 0

    print("Problem (d)\n")
    for method in methods:
        err = TOL[i]
        result = method(x0, funcs, jacob, err)
        print(f"{method.__name__} method with x0 = {x0}:")
        print(result)
        print("")
        i += 1


if __name__ == "__main__":
    main()
