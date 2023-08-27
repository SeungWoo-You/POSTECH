import os
import numpy as np
import sympy as sym
from types import FunctionType
import time

from methods import tri_poly, FFT


def problem_b(func: FunctionType, interval: tuple, m: int) -> None:
    x = np.array([interval[0] + (interval[1] - interval[0]) /
                 (2 * m) * j for j in range(2 * m)])
    x = np.interp(x, interval, (-np.pi, np.pi))
    y = func(x)
    c = FFT(y)[:m + 1]

    a = c.real / m
    np.put(a, [0, -1], [a[0] / 2, a[-1] / 2])
    b = c.imag / m
    for i in range(m + 1):
        a[i] = a[i] * (-1)**i
        try:
            b[i] = b[i] * (-1)**i
        except:
            pass
    b = np.delete(b, [0, -1])
    print(f"coeff a: {a}")
    print(f"coeff b: {b}")


def main():
    os.system("cls")
    x = sym.symbols('x')
    def func(x): return x * (np.pi - x)
    expr = x * (sym.pi - x)
    deg = 4
    interval = (-sym.pi, sym.pi)

    start = time.time()
    # Problem (a)
    tri_aprx = tri_poly(expr, interval, deg, cont=False, m=deg)  # direct calc
    print(f"interpolate {func.__name__} {expr} directly")
    print(tri_aprx)
    end = time.time()
    print(f"calc time: {end - start}")
    print("")

    # Problem (b)
    start = time.time()
    print("use FFT")
    problem_b(func, (-np.pi, np.pi), deg)
    end = time.time()
    print(f"calc time: {end - start}")


if __name__ == "__main__":
    main()
