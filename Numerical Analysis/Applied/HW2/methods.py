import numpy as np
import pandas as pd
from types import FunctionType
import sympy as sym


def RKF(t: float, w: float, h: float, f: FunctionType) -> tuple[float, float]:
    k1 = h * f(t, w)
    k2 = h * f(t + h / 4, w + 1 / 4 * k1)
    k3 = h * f(t + 3 * h / 8, w + 3 / 32 * k1 + 9 / 32 * k2)
    k4 = h * f(t + 12 * h / 13, w + 1932 / 2197 *
               k1 - 7200 / 2197 * k2 + 7296 / 2197 * k3)
    k5 = h * f(t + h, w + 439 / 216 * k1 - 8 * k2 +
               3680 / 513 * k3 - 845 / 4104 * k4)
    k6 = h * f(t + h / 2, w - 8 / 27 * k1 + 2 * k2 - 3544 /
               2565 * k3 + 1859 / 4104 * k4 - 11 / 40 * k5)

    R = 1 / h * abs(1 / 360 * k1 - 128 / 4275 * k3 - 2197 /
                    75240 * k4 + 1 / 50 * k5 + 2 / 55 * k6)
    W = w + 25 / 216 * k1 + 1408 / 2565 * k3 + 2197 / 4104 * k4 - 1 / 5 * k5
    return (R, W)


def AM2(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    W = sym.Symbol('x')
    equation = sym.Eq(
        W, w[-1] + h / 12 * (5 * f(t[-1] + h, W) + 8 * f(t[-1], w[-1]) - f(t[-2], w[-2])))
    return np.float64(sym.solve(equation)[0])


def AM3(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    W = sym.Symbol('x')
    equation = sym.Eq(W, w[-1] + h / 24 * (9 * f(t[-1] + h, W) + 19 *
                      f(t[-1], w[-1]) - 5 * f(t[-2], w[-2]) + f(t[-3], w[-3])))
    return np.float64(sym.solve(equation)[0])


def AM3_modified(w: list[float], h: float, f: FunctionType) -> float:
    return w[-1] + h / 24 * (9 * f(w[-1]) + 19 * f(w[-1]) - 5 * f(w[-2]) + f(w[-3]))


def AM4(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    W = sym.Symbol('x')
    equation = sym.Eq(W, w[-1] + h / 720 * (251 * f(t[-1] + h, W) + 646 * f(t[-1], w[-1]) -
                      264 * f(t[-2], w[-2]) + 106 * f(t[-3], w[-3]) - 19 * f(t[-4], w[-4])))
    return np.float64(sym.solve(equation)[0])


def AB2(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    return w[-1] + h / 2 * (3 * f(t[-1], w[-1]) - f(t[-2], w[-2]))


def AB3(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    return w[-1] + h / 12 * (23 * f(t[-1], w[-1]) - 16 * f(t[-2], w[-2]) + 5 * f(t[-3], w[-3]))


def AB4(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    return w[-1] + h / 24 * (55 * f(t[-1], w[-1]) - 59 * f(t[-2], w[-2]) + 37 * f(t[-3], w[-3]) - 9 * f(t[-4], w[-4]))


def AB5(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    return w[-1] + h / 720 * (1901 * f(t[-1], w[-1]) - 2774 * f(t[-2], w[-2]) + 2616 * f(t[-3], w[-3]) - 1274 * f(t[-4], w[-4]) + 251 * f(t[-5], w[-5]))


def PC(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    w_p = AB4(t, w, h, f)
    W = w[-1] + h / 24 * (9 * f(t[-1] + h, w_p) + 19 *
                          f(t[-1], w[-1]) - 5 * f(t[-2], w[-2]) + f(t[-3], w[-3]))
    return W


def RK4(t: float, w: float, h: float, f: FunctionType) -> float:
    k1 = h * f(t, w)
    k2 = h * f(t + h / 2, w + 1 / 2 * k1)
    k3 = h * f(t + h / 2, w + 1 / 2 * k2)
    k4 = h * f(t + h, w + k3)
    return w + 1 / 6 * (k1 + 2 * k2 + 2 * k3 + k4)


def Newton_modified(w: list[float], h: float, f: FunctionType) -> float:
    func = w[-1] - AM3_modified(w, h, f)
    deriv = 1 - h * 9 / 24 * np.exp(w[-1])
    return w[-1] - func / deriv


def mk_result(t: list[float], w: list[float], exact_func: FunctionType) -> pd.DataFrame:
    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_func(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])
    return result
