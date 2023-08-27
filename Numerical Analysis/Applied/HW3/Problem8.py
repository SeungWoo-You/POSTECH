import os
import sympy as sym

from methods import tri_poly


def main():
    os.system("cls")
    x = sym.symbols('x')
    f = sym.exp(x)
    deg = 3
    interval = (-sym.pi, sym.pi)

    print(
        f"with symbols: {tri_poly(f, interval, deg, cont=True, raw=True)}")
    print(f"with values: {tri_poly(f, interval, deg, cont=True)}")


if __name__ == "__main__":
    main()
