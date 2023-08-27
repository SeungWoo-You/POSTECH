import os
import numpy as np
import pandas as pd


t_start: float = 1
t_end: float = 2
h: float = 0.1
y_start: float = 0


def func(t: float, y: float) -> float:
    return 2 / t * y + t**2 * np.exp(t)


def deriv_2(t: float) -> float:
    return (t**2 + 4 * t + 2) * np.exp(t) - 2 * np.exp(1)


def exact_sol(t: float | pd.Series) -> float | pd.Series:
    return t**2 * (np.exp(t) - np.exp(1))


def Euler(t: float, w: float) -> float:
    return w + h * func(t, w)


def problem_a() -> list:
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(Euler(t[i], w[i]))
        t.append(t[i] + h)
        i += 1

    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_sol(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])
    print("problem (a)")
    print(result, "\n")

    return w


def linear_interpolation(d1: float, d2: float, y1: float, y2: float) -> float:
    # get y_axis information from given x position
    # d1: x-distance between (x1, y1) and target (x, y)
    # d2: x-distance between (x2, y2) and target (x, y)
    return d2 / (d1 + d2) * y1 + d1 / (d1 + d2) * y2


def find_start_t(time: float) -> float:
    t = t_start
    while np.isclose(t, time) == False:
        if round(abs(t - time), 2) <= h:
            return t
        t += h
    return t


def problem_b(w: list) -> None:
    time = [1.04, 1.55, 1.97]
    result = pd.DataFrame(time, columns=['t'])
    aprx = []

    for t in time:
        t1 = find_start_t(t)
        i = round((t1 - t_start) / h)
        w1 = w[i]
        t2 = t1 + h
        w2 = w[i + 1]

        aprx_y = linear_interpolation(np.abs(t1 - t), np.abs(t2 - t), w1, w2)
        aprx.append(aprx_y)

    result['aprx'] = aprx
    result['exact'] = exact_sol(result['t'])
    result['error'] = np.abs(result['exact'] - result['aprx'])

    print("problem (b)")
    print(result, "\n")


def problem_c() -> None:
    L = 2
    t = t_end
    a = t_start
    M = deriv_2(2)

    result = 0.1 / (M / (2 * L) * (np.exp(L * (t - a)) - 1))

    print("problem (c)")
    print(result)


def main():
    os.system("cls")
    w = problem_a()
    problem_b(w)
    problem_c()


if __name__ == "__main__":
    main()
