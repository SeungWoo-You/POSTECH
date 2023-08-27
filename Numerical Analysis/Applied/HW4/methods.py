import numpy as np
import pandas as pd
from types import FunctionType


def idx(i: int) -> int:
    return -1 + i


def FPI(x0: np.ndarray, func: np.ndarray, stop_err: float) -> pd.DataFrame:
    n = len(x0)
    m = len(func)
    x = x0
    err = [1]
    i = 0
    x_data = pd.DataFrame(columns=[f'x_{i}' for i in range(1, n + 1)])
    x_data.loc[0] = x0

    while not err[-1] < stop_err:
        x_new = np.array([func[j](x) for j in range(m)])
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


def Seidel(x0: np.ndarray, func: np.ndarray, stop_err: float) -> pd.DataFrame:
    n = len(x0)
    m = len(func)
    x = x0
    err = [1]
    i = 0
    x_data = pd.DataFrame(columns=[f'x_{i}' for i in range(1, n + 1)])
    x_data.loc[0] = x0

    while not err[-1] < stop_err:
        x_new = x.copy()
        for j in range(m):
            x_new[j] = func[j](x_new)
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


def Newton(x0: np.ndarray, func: np.ndarray, deriv: FunctionType, stop_err: float) -> pd.DataFrame:
    n = len(x0)
    m = len(func)
    x = x0
    err = [1]
    i = 0
    x_data = pd.DataFrame(columns=[f'x_{i}' for i in range(1, n + 1)])
    x_data.loc[0] = x0

    while not err[-1] < stop_err:
        F = np.array([func[j](x) for j in range(m)])
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


def Broyden(x0: np.ndarray, func: np.ndarray, deriv: FunctionType, stop_err: float) -> pd.DataFrame:
    n = len(x0)
    m = len(func)
    x = x0
    err = [1]
    i = 0
    x_data = pd.DataFrame(columns=[f'x_{i}' for i in range(1, n + 1)])
    x_data.loc[0] = x0

    A_inv = np.linalg.inv(deriv(x0))
    def F(x): return np.array([func[j](x) for j in range(m)])

    while not err[-1] < stop_err:
        x_new = x - np.matmul(A_inv, F(x))
        y = F(x_new) - F(x)
        s = x_new - x
        A_inv = Sherman_Morrison(A_inv, s, y)
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


def matmul_multi(x: list) -> np.ndarray:
    A = x[0]
    del x[0]
    for mat in x:
        A = np.matmul(A, mat)
    return A


def Sherman_Morrison(A_inv: np.ndarray, s: np.ndarray, y: np.ndarray) -> np.ndarray:
    s_trans = np.transpose(s)
    sAy = matmul_multi([s_trans, A_inv, y])
    Ay = np.matmul(A_inv, y)
    sA = np.matmul(s_trans, A_inv)
    vecmul = np.matmul(np.array([s - Ay]).T, np.array([sA]))
    return A_inv + vecmul / sAy


def Steepest_Descent(x0: np.ndarray, func: np.ndarray, deriv: FunctionType, stop_err: float) -> np.ndarray:
    n = len(x0)
    m = len(func)
    x = x0
    err = [1]
    i = 0
    x_data = pd.DataFrame(columns=[f'x_{i}' for i in range(1, n + 1)])
    x_data.loc[0] = x0

    z = 0
    def F(x): return np.array([func[j](x) for j in range(m)])
    def g(x): return np.sum(F(x)**2)
    g_data = [g(x0)]

    while not err[-1] < stop_err:
        grad_g = 2 * np.matmul(deriv(x).T, F(x))
        z_0 = np.linalg.norm(grad_g, ord=2)
        z = 1 / z_0 * grad_g
        a1 = 0
        a3 = 1
        g1 = g(x - a1 * z)
        g3 = g(x - a3 * z)
        while not g3 < g1:
            a3 /= 2
            g3 = g(x - a3 * z)
            if a3 < stop_err / 2:
                x_data['g(x)'] = g_data
                x_data['error'] = err
                x_data.at[0, 'error'] = None
                return x_data
        a2 = a3 / 2
        g2 = g(x - a2 * z)
        h1 = (g2 - g1) / a2
        h2 = (g3 - g2) / (a3 - a2)
        h3 = (h2 - h1) / a3
        a0 = 0.5 * (a2 - h1 / h3)
        g0 = g(x - a0 * z)
        a_new = a0 if g0 == min(g0, g3) else a3
        x_new = x - a_new * z

        err.append(np.linalg.norm(x_new - x, ord=np.inf))
        g_data.append(g(x_new))
        x = x_new
        i += 1
        x_data.loc[i] = x
        if np.abs(g_data[-1] - g_data[-2]) < stop_err:
            break
        if i > 123456:
            print("Looped!")
            break

    x_data['g(x)'] = g_data
    x_data['error'] = err
    x_data.at[0, 'error'] = None
    return x_data


def Continuation(x0: np.ndarray, func: np.ndarray, deriv: FunctionType, N: int, method: FunctionType) -> np.ndarray:
    m = len(func)
    x = x0
    h = 1 / N
    for i in range(N):
        F = np.array([func[j](x) for j in range(m)])
        x = method(x, h, F, deriv)
    return x


def Euler(w: np.ndarray, h: float, f: np.ndarray, deriv: FunctionType) -> np.ndarray:
    A = deriv(w)
    k1 = -np.linalg.solve(A, f)
    return w + h * k1


def RK4(w: np.ndarray, h: float, f: np.ndarray, deriv: FunctionType) -> np.ndarray:  # not use
    A = deriv(w)
    k1 = -h * np.linalg.solve(A, f)
    A = deriv(w + 1 / 2 * k1)
    k2 = -h * np.linalg.solve(A, f)
    A = deriv(w + 1 / 2 * k2)
    k3 = -h * np.linalg.solve(A, f)
    A = deriv(w + k3)
    k4 = -h * np.linalg.solve(A, f)
    return (k1 + 2 * k2 + 2 * k2 + k4) / 6
