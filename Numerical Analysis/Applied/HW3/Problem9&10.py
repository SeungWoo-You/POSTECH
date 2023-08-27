import os
import numpy as np
import sympy as sym

from methods import tri_poly, mk_result


def main():
    os.system("cls")
    x = sym.symbols('x')
    def func(x): return np.exp(x) * np.cos(2 * x)
    expr = sym.exp(x) * sym.cos(2 * x)
    deg = 3
    interval = (-sym.pi, sym.pi)

    t = np.array([-np.pi + 0.2 * j * np.pi for j in range(11)])
    m = [4, 8]  # Problem 9 & 10
    for m_i in m:
        tri_aprx = tri_poly(expr, interval, deg, cont=False, m=m_i)
        print(f"approximation with m = {m_i}")
        print(tri_aprx)
        w = [tri_aprx.subs(x, t_i) for t_i in t]
        print(mk_result(t, np.array(w, np.float64), func))
        print("")


if __name__ == "__main__":
    main()
