import os
import numpy as np
import pandas as pd


t_start: float = 1
t_end: float = 2
h: float = 0.25
y_start: float = 2


def func(t: float, y: float) -> float:
    return 1 + y / t


def exact_sol(t: float | pd.Series) -> float | pd.Series:
    return t * np.log(t) + 2 * t


def mod_Euler(t: float, w: float) -> float:
    E1 = func(t, w)
    E2 = func(t + h, w + h * E1)
    return w + h / 2 * (E1 + E2)


def midpoint(t: float, w: float) -> float:
    m1 = w + h / 2 * func(t, w)
    return w + h * func(t + h / 2, m1)


def RK4(t: float, w: float) -> float:
    k1 = h * func(t, w)
    k2 = h * func(t + h / 2, w + 1 / 2 * k1)
    k3 = h * func(t + h / 2, w + 1 / 2 * k2)
    k4 = h * func(t + h, w + k3)
    return w + 1 / 6 * (k1 + 2 * k2 + 2 * k3 + k4)


def problem_1() -> None:
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(mod_Euler(t[i], w[i]))
        t.append(t[i] + h)
        i += 1

    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_sol(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])

    print("problem 1-(a)")
    print(result, "\n")


def problem_5() -> None:
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(midpoint(t[i], w[i]))
        t.append(t[i] + h)
        i += 1

    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_sol(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])

    print("problem 5-(a)")
    print(result, "\n")


def problem_13() -> None:
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(RK4(t[i], w[i]))
        t.append(t[i] + h)
        i += 1

    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_sol(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])

    print("problem 13-(a)")
    print(result, "\n")


def main():
    os.system("cls")
    problem_1()
    problem_5()
    problem_13()


if __name__ == "__main__":
    main()
