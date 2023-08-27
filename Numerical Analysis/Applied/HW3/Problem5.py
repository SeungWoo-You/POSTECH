import os
import numpy as np

from methods import Cheby_interpol


def problem_b() -> None:
    def f(x): return np.sin(x)
    interval = (-1, 1)
    n = 2
    coef = Cheby_interpol(f, interval, n)
    print(f"coefficient of problem (b) with deg{n}: {coef}")
    print("")


def problem_d() -> None:
    def f(x): return x**4
    interval = (-1, 1)
    n = 2
    coef = Cheby_interpol(f, interval, n)
    print(f"coefficient of problem (d) with deg{n}: {coef}")
    print("")


def main():
    os.system("cls")
    problem_b()
    problem_d()


if __name__ == "__main__":
    main()
