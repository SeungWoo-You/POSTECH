import os
import numpy as np
import sympy as sym

from methods import Pade_aprx, mk_result


def main():
    os.system("cls")
    x = sym.symbols('x')
    f = x * sym.ln(x + 1)
    def func(x): return x * np.log(x + 1)
    t = np.array([0.2 * i for i in range(1, 6)])
    deg = 3

    for n in range(deg + 1):
        m = deg - n
        pade = Pade_aprx(f, n, m)
        if pade == None:
            print(
                f"There is no pade approximation with (n, m) = {(n, m)}. Take p(x) = 0.")
            pade = 0 * x
        else:
            print(f"pade approximation with (n, m) = {(n, m)}\n{pade}")

        w = []
        for t_i in t:
            w.append(pade.subs(x, t_i))
        print(mk_result(t, np.array(w, np.float64), func))
        print("")


if __name__ == "__main__":
    main()
