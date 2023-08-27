import os
import numpy as np
import pandas as pd


t_start: float = 1
t_end: float = 2
h: float = 0.05
y_start: float = -1


def func(t: float, y: float) -> float:
    return 1 / t**2 - y / t - y**2


def deriv_2(t: float) -> float:
    return -2 / t**3


def deriv_3(t: float) -> float:
    return 6 / t**4


def deriv_4(t: float) -> float:
    return -24 / t**5


def exact_sol(t: float | np.ndarray) -> float | np.ndarray:
    return -1 / t


def Taylor2(t: float, w: float) -> float:
    return w + h * (func(t, w) + h / 2 * deriv_2(t))


def Taylor4(t: float, w: float) -> float:
    return w + h * (func(t, w) + h / 2 * deriv_2(t) + h**2 / 6 * deriv_3(t) + h**3 / 24 * deriv_4(t))


def problem_a() -> list:
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(Taylor2(t[i], w[i]))
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
        if round(abs(t - time), 3) <= h:
            return t
        t += h
    return t


def problem_b(w: list) -> None:
    time = [1.052, 1.555, 1.978]
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
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(Taylor4(t[i], w[i]))
        t.append(t[i] + h)
        i += 1

    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_sol(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])
    print("problem (c)")
    print(result)


def main():
    os.system("cls")
    w = problem_a()
    problem_b(w)
    problem_c()


if __name__ == "__main__":
    main()
