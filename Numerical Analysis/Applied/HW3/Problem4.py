import os
import sympy as sym

from methods import LS_ortho


def problem_a() -> None:
    x = sym.symbols('x')
    f = x**2
    for i in range(1, 4):
        coef = LS_ortho(f, i)
        print(f"coefficient of problem (a) with deg{i}: {coef}")
    print("")


def problem_b() -> None:
    x = sym.symbols('x')
    f = sym.exp(-x)
    for i in range(1, 4):
        coef = LS_ortho(f, i)
        print(f"coefficient of problem (b) with deg{i}: {coef}")
    print("")


def problem_c() -> None:
    x = sym.symbols('x')
    f = x**3
    for i in range(1, 4):
        coef = LS_ortho(f, i)
        print(f"coefficient of problem (c) with deg{i}: {coef}")
    print("")


def problem_d() -> None:
    x = sym.symbols('x')
    f = sym.exp(-2 * x)
    for i in range(1, 4):
        coef = LS_ortho(f, i)
        print(f"coefficient of problem (d) with deg{i}: {coef}")
    print("")


def main():
    os.system("cls")
    problem_a()
    problem_b()
    problem_c()
    problem_d()


if __name__ == "__main__":
    main()
