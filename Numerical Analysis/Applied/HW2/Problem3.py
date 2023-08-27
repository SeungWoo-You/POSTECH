import os
import sys
import numpy as np
from types import FunctionType

from methods import AM3_modified, Newton_modified, mk_result


def func_a(y: float) -> float:
    return np.exp(y)


def exact_sol_a(t: float | np.ndarray) -> float | np.ndarray:
    return 1 - np.log(1 - np.exp(1) * t)


def problem_a(step: int, method: FunctionType) -> None:
    t_start = 0
    t_end: float = 0.20
    h: float = 0.01
    t = [t_start + h * i for i in range(step)]
    w = list(exact_sol_a(np.array(t)))

    while t[-1] + sys.float_info.epsilon < t_end:
        w.append(method(w, h, func_a))
        t.append(t[-1] + h)
    result = mk_result(t, w, exact_sol_a)
    print("problem 3 - (a)")
    print(f"{result}\n")


def problem_c(step: int, method: FunctionType) -> None:
    t_start = 0
    t_end: float = 0.20
    h: float = 0.01
    t = [t_start + h * i for i in range(step)]
    w = list(exact_sol_a(np.array(t)))

    while t[-1] + sys.float_info.epsilon < t_end:
        w.append(method(w, h, func_a))
        t.append(t[-1] + h)
    result = mk_result(t, w, exact_sol_a)
    print("problem 3 - (c)")
    print(f"{result}\n")


def main():
    os.system("cls")
    problem_a(3, AM3_modified)
    problem_c(3, Newton_modified)


if __name__ == "__main__":
    main()
