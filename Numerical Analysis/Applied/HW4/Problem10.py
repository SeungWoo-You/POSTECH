import os
import numpy as np
import pandas as pd
from types import FunctionType

from methods import Euler, idx

g = 32.17
mu = 0
X = 2
del_y = 0.2
N = 20
v_0 = 0


def f(x: np.ndarray, n: int) -> float:
    return term(x, n + 1) - term(x, n)


def term(x: np.ndarray, n: int) -> float:
    return np.sin(x[idx(n)]) / v(x, n) * (1 - mu * w(x, n))


def deriv_term(x: np.ndarray, n: int) -> float:
    return np.cos(x[idx(n)]) / v(x, n) * (1 - mu * w(x, n))


def v(x: np.ndarray, n: int) -> float:
    T = np.array([1 / np.cos(x[idx(i)]) for i in range(1, n + 1)])
    square = v_0**2 + 2 * g * n * del_y - 2 * mu * del_y * np.sum(T)
    return np.sqrt(square)


def w(x: np.ndarray, n: int) -> float:
    T = np.array([1 / (v(x, i)**3 * np.cos(x[idx(i)]))
                 for i in range(1, N + 1)])
    return -del_y * v(x, n) * np.sum(T)


def f_N(x: np.ndarray) -> float:
    T = np.array(np.tan(x))
    return del_y * np.sum(T) - X


def jacob(x: np.ndarray) -> np.ndarray:
    result = np.array([])
    for j in range(1, N):
        i = idx(j)
        new_row = np.zeros((N, ), dtype=np.float64)
        new_row[i] = -deriv_term(x, j)
        new_row[i + 1] = deriv_term(x, j + 1)
        if result.size == 0:
            result = np.array(new_row)
        else:
            result = np.vstack([result, new_row])
    new_row = del_y / np.cos(x)
    result = np.vstack([result, new_row])
    return result


def Continuation(x0: np.ndarray, func: np.ndarray, deriv: FunctionType, N: int, method: FunctionType) -> np.ndarray:
    x = x0
    h = 1 / N
    for i in range(N):
        F = np.array([func[0](x, n) for n in range(1, N)] + [func[1](x)])
        x = method(x, h, F, deriv)
    return x


def Newton(x0: np.ndarray, func: np.ndarray, deriv: FunctionType, stop_err: float) -> pd.DataFrame:
    x = x0
    err = [1]
    i = 0
    x_data = pd.DataFrame(columns=[f'O_{i}' for i in range(1, N + 1)])
    x_data.loc[0] = x0

    while not err[-1] < stop_err:
        F = np.array([func[0](x, n) for n in range(1, N)] + [func[1](x)])
        x_new = x - np.linalg.solve(deriv(x), F)
        err.append(np.linalg.norm(x_new - x, ord=np.inf))
        x = x_new
        i += 1
        x_data.loc[i] = x
        if i > 123456:
            print("Looped!")
            break

    x_data['error'] = err
    x_data.at[0, 'error'] = None
    return x_data


def main():
    os.system("cls")
    x0 = np.array([0 for i in range(N)])
    funcs = np.array([f, f_N])
    err = 10**(-2)

    better_x0 = Continuation(x0, funcs, jacob, N, Euler)
    result = Newton(better_x0, funcs, jacob, err)
    print(f"Newton method with O_0 = {better_x0}:")
    print(result.transpose())


if __name__ == "__main__":
    main()
