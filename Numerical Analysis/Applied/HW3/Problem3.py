import os
import sympy as sym

from methods import LS_poly


def problem_a() -> None:
    x = sym.symbols('x')
    f = x**2 + 3 * x + 2
    interval = (0, 1)

    coef = LS_poly(interval, f, 2, cont=True)
    print(f"coefficient of problem (a): {coef}")


def problem_c() -> None:
    x = sym.symbols('x')
    f = 1 / x
    interval = (1, 3)

    coef = LS_poly(interval, f, 2, cont=True)
    print(f"coefficient of problem (c): {coef}")


def problem_e() -> None:
    x = sym.symbols('x')
    f = 1 / 2 * sym.cos(x) + 1 / 3 * sym.sin(2 * x)
    interval = (0, 1)

    coef = LS_poly(interval, f, 2, cont=True)
    print(f"coefficient of problem (e): {coef}")


def main():
    os.system("cls")
    problem_a()
    problem_c()
    problem_e()


if __name__ == "__main__":
    main()
