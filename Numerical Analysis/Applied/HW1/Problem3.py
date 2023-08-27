import os
import numpy as np
import pandas as pd


t_start: float = 0
t_end: float = 5
y_start: float = 1


def func(t: float, y: float) -> float:
    return -y + t + 1


def exact_sol(t: float | pd.Series) -> float | pd.Series:
    return np.exp(-t) + t


def Euler(t: float, w: float, h: float) -> float:
    return w + h * func(t, w)


def problem_a(h: float) -> None:
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(Euler(t[i], w[i], h))
        t.append(t[i] + h)
        i += 1

    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_sol(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])

    print("problem (a)")
    # if you want to display all data, then remove below line or '10' to 'None'
    with pd.option_context('display.max_rows', 10):
        print(result, "\n")


def problem_b() -> None:
    delta = 10**(-6)
    M = np.exp(0)

    print("problem (b)")
    print(np.sqrt(2 * delta / M))


def main():
    os.system("cls")
    h_list = [0.2, 0.1, 0.005]

    for h in h_list:
        problem_a(h)

    problem_b()


if __name__ == "__main__":
    main()
