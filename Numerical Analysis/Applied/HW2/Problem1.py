import os
import numpy as np
from types import FunctionType

from methods import RKF, mk_result


h_max: float = 0.25
h_min: float = 0.05
TOL: float = 10**(-4)


def func_a(t: float, y: float) -> float:
    return t * np.exp(3 * t) - 2 * y


def exact_sol_a(t: float) -> float:
    return 1 / 5 * t * np.exp(3 * t) - 1 / 25 * np.exp(3 * t) + 1 / 25 * np.exp(-2 * t)


def func_c(t: float, y: float) -> float:
    return 1 + y / t


def exact_sol_c(t: float) -> float:
    return t * np.log(t) + 2 * t


def find_h(h: float, q: float) -> float:
    if q <= 0.1 * h:  # step size of RK4
        h = 0.1 * h
    elif q > 4:  # step of RK4
        h = 4 * h
    else:
        h = q * h
    if h > h_max:
        h = h_max
    return h


def problem_a() -> None:
    t_start: float = 0
    t_end: float = 1
    y_start: float = 0
    h: float = h_max
    t = [float(t_start)]
    w = [y_start]
    flag: bool = True
    q_coef: float = (1 / 2)**(1 / 4)
    i = 0

    while flag:
        R, W = RKF(t[i], w[i], h, func_a)
        if R <= TOL:
            t.append(t[i] + h)
            w.append(W)
            i += 1
        q: float = q_coef * (TOL / R)**(1 / 4)
        h = find_h(h, q)
        if t[i] >= t_end:
            flag = False
        elif t[i] + h > t_end:
            h = t_end - t[i]
        elif h < h_min:
            flag = False
            print("Cannot complete the Process: minimum h was exceeded.")
            print(f"h: {h}, t: {t[i]}")
            return 0
    result = mk_result(t, w, exact_sol_a)
    print("problem 1 - (a)")
    print(f"{result}\n")
    return


def problem_c() -> None:
    t_start: float = 1
    t_end: float = 2
    y_start: float = 2
    h: float = h_max
    t = [float(t_start)]
    w = [y_start]
    flag: bool = True
    q_coef: float = (1 / 2)**(1 / 4)
    i = 0

    while flag:
        R, W = RKF(t[i], w[i], h, func_c)
        if R <= TOL:
            t.append(t[i] + h)
            w.append(W)
            i += 1
        q: float = q_coef * (TOL / R)**(1 / 4)
        h = find_h(h, q)
        if t[i] >= t_end:
            flag = False
        elif t[i] + h > t_end:
            h = t_end - t[i]
        elif h < h_min:
            flag = False
            print("Cannot complete the Process: minimum h was exceeded.")
            print(f"h: {h}, t: {t}")
            return 0
    result = mk_result(t, w, exact_sol_c)
    print("problem 1 - (c)")
    print(f"{result}\n")
    return


def main():
    os.system("cls")
    problem_a()
    problem_c()


if __name__ == "__main__":
    main()
