import os
import numpy as np

from methods import Steepest_Descent, Newton, idx


def f1(x: np.ndarray) -> float:
    return np.log(x[idx(1)]**2 + x[idx(2)]**2) - np.sin(x[idx(1)] * x[idx(2)]) - np.log(2) - np.log(np.pi)


def f2(x: np.ndarray) -> float:
    return np.exp(x[idx(1)] - x[idx(2)]) + np.cos(x[idx(1)] * x[idx(2)])


def jacob(x: np.ndarray) -> np.ndarray:
    return np.array([
        [2 * x[idx(1)] / (x[idx(1)]**2 + x[idx(2)] ** 2) - np.cos(x[idx(1)] * x[idx(2)]) * x[idx(2)],
         2 * x[idx(2)] / (x[idx(1)]**2 + x[idx(2)] ** 2) - np.cos(x[idx(1)] * x[idx(2)]) * x[idx(1)]],
        [np.exp(x[idx(1)] - x[idx(2)]) - np.sin(x[idx(1)] * x[idx(2)]) * x[idx(2)],
         -np.exp(x[idx(1)] - x[idx(2)]) - np.sin(x[idx(1)] * x[idx(2)]) * x[idx(1)]]
    ])


def main():
    os.system("cls")
    x0 = np.array([2, 2], dtype=np.float64)
    funcs = np.array([f1, f2])
    TOL = [0.05, 10**(-6)]
    methods = [Steepest_Descent, Newton]
    i = 0

    print("Problem (c)\n")
    for method in methods:
        err = TOL[i]
        result = method(x0, funcs, jacob, err)
        print(f"{method.__name__} method with x0 = {x0}:")
        print(result)
        print("")
        i += 1


if __name__ == "__main__":
    main()
