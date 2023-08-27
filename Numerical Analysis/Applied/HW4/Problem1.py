import os
import numpy as np

from methods import FPI, Seidel, idx


def g1(x: np.ndarray) -> float:
    return (x[idx(1)]**2 + x[idx(2)]**2 + 8) / 10


def g2(x: np.ndarray) -> float:
    return (x[idx(1)] * x[idx(2)]**2 + x[idx(1)] + 8) / 10


def problem_b(x0: np.ndarray, funcs: np.ndarray, err: float) -> None:
    result = FPI(x0, funcs, err)
    print(f"Fixed point iteration with x0 = {x0}:")
    print(result)
    print("")


def problem_c(x0: np.ndarray, funcs: np.ndarray, err: float) -> None:
    result = Seidel(x0, funcs, err)
    print(f"Gauss-Seidel method with x0 = {x0}:")
    print(result)
    print("")


def main():
    os.system("cls")
    x0 = np.array([0, 0], dtype=np.float64)
    funcs = np.array([g1, g2])
    err = 10**(-5)

    problem_b(x0, funcs, err)
    problem_c(x0, funcs, err)


if __name__ == "__main__":
    main()
