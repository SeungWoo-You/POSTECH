import os
import sys
import numpy as np
from types import FunctionType
import sympy as sym

from methods import AB2, AB3, AB4, AB5, AM2, AM3, AM4, PC, RK4, mk_result


def func_a(t: float, y: float | sym.Expr) -> float | sym.Expr:
    return t * np.exp(3 * t) - 2 * y


def exact_sol_a(t: float | np.ndarray) -> float | np.ndarray:
    return 1 / 5 * t * np.exp(3 * t) - 1 / 25 * np.exp(3 * t) + 1 / 25 * np.exp(-2 * t)


def func_c(t: float, y: float) -> float:
    return 1 + y / t


def exact_sol_c(t: float) -> float:
    return t * np.log(t) + 2 * t


def problem_a(step: int, method: FunctionType) -> None:
    t_start = 0
    t_end: float = 1
    h: float = 0.2
    if id(method) == id(PC):
        t = [t_start]
        w = [exact_sol_a(t[-1])]
        for i in range(step):
            w.append(RK4(t[-1], w[-1], h, func_a))
            t.append(t[-1] + h)
    else:
        t = [t_start + h * i for i in range(step)]
        w = list(exact_sol_a(np.array(t)))

    while t[-1] + sys.float_info.epsilon < t_end:
        w.append(method(t, w, h, func_a))
        t.append(t[-1] + h)
    result = mk_result(t, w, exact_sol_a)
    print(f"problem 2 - (a): {method.__name__} method")
    print(f"{result}\n")
    return


def problem_c(step: int, method: FunctionType) -> None:
    t_start = 1.0
    t_end: float = 2.0
    h: float = 0.2
    if id(method) == id(PC):
        t = [t_start]
        w = [exact_sol_c(t[-1])]
        for i in range(step):
            w.append(RK4(t[-1], w[-1], h, func_c))
            t.append(t[-1] + h)
    else:
        t = [t_start + h * i for i in range(step)]
        w = list(exact_sol_c(np.array(t)))

    while t[-1] + sys.float_info.epsilon < t_end:
        w.append(method(t, w, h, func_c))
        t.append(t[-1] + h)
    result = mk_result(t, w, exact_sol_c)
    print(f"problem 2 - (c): {method.__name__} method")
    print(f"{result}\n")
    return


def main():
    os.system("cls")
    scheme_dict = {AB2: 2, AB3: 3, AB4: 4,
                   AB5: 5, AM2: 2, AM3: 3, AM4: 4, PC: 4}
    problems = [problem_a, problem_c]

    for problem in problems:
        for method, step in scheme_dict.items():
            problem(step, method)


if __name__ == "__main__":
    main()
