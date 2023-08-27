import numpy as np
import pandas as pd
import sympy as sym
from types import FunctionType


def LS_poly(x: np.ndarray | tuple[float, float], y: np.ndarray | sym.Expr, n: int, cont: bool) -> np.ndarray:
    X = np.empty((0, n + 1), dtype=np.float64)
    Y = np.array([])
    t = sym.symbols('x')
    interval = list(x)
    interval.insert(0, t)

    for i in range(n + 1):
        if cont == True:
            insert_x = [[np.float64(sym.integrate(t**(i + j), interval))
                         for j in range(n + 1)]]
        else:
            insert_x = [[np.sum(x**(i + j)) for j in range(n + 1)]]
        X = np.append(X, np.array(insert_x), axis=0)
    if cont == True:
        insert_y = [np.float64(sym.integrate(t**j * y, interval))
                    for j in range(n + 1)]
    else:
        insert_y = [np.sum(x**j * y) for j in range(n + 1)]
    Y = np.append(Y, np.array(insert_y), axis=0)
    A = np.flipud(np.linalg.solve(X, Y))
    return A


def LSE(x: np.ndarray, y: np.ndarray, p: np.poly1d) -> float:
    poly = np.array([p(x_i) for x_i in x])
    error = np.sum((y - poly) ** 2)
    return error


def LS_ortho(f: sym.Expr, n: int) -> np.ndarray:
    X = np.empty((0, n + 1), dtype=np.float64)
    Y = np.array([])
    x = sym.symbols('x')
    L0 = 1
    L1 = -x + 1
    L2 = 1 / 2 * (x**2 - 4 * x + 2)
    L3 = 1 / 6 * (-x**3 + 9 * x**2 - 18 * x + 6)
    L = [L0, L1, L2, L3]
    w = sym.exp(-x)
    interval = [x, 0, np.inf]

    for i in range(n + 1):
        insert_x = [
            [np.float64(sym.integrate(w * L[i] * L[j], interval)) for j in range(n + 1)]]
        X = np.append(X, np.array(insert_x), axis=0)
    insert_y = [np.float64(sym.integrate(w * f * L[j], interval))
                for j in range(n + 1)]
    Y = np.append(Y, np.array(insert_y), axis=0)
    A = np.linalg.solve(X, Y)

    eq = 0
    for i in range(n + 1):
        eq += A[i] * L[i]
    eq = sym.Poly(eq, x)
    res = np.array(eq.all_coeffs(), dtype=np.float64)
    return res


def Cheby_interpol(f: FunctionType, interval: tuple, n: int) -> np.ndarray:
    x = sym.symbols('x')
    T = np.array([np.cos((2 * k + 1) / (2 * (n + 1)) * np.pi)
                  for k in range(n + 1)])
    nodes = np.interp(T, interval, (-1, 1))
    y = f(nodes)
    print(f"nodes of T{n + 1}: {nodes}")

    Lags = []
    for i in range(n + 1):  # instead of this, use sci.lagrange(nodes, y) to get the same result
        L_coef = nodes.copy()
        L = x - L_coef
        L = np.delete(L, i, axis=0)
        L = sym.prod(L)
        L_coef = L_coef[i] - L_coef
        L_coef = np.delete(L_coef, i, axis=0)
        L_coef = sym.prod(L_coef)
        eq = sym.Poly(L / L_coef, x)
        Lags.append(eq)
    poly = np.sum(y * np.array(Lags))
    poly = sym.Poly(poly,  x)
    res = np.array(poly.all_coeffs(), dtype=np.float64)
    return res


def Pade_aprx(f: sym.Expr, n: int, m: int) -> sym.Expr | None:
    x = sym.symbols('x')
    p = sym.symbols(f"p:{n + 1}")
    q = sym.symbols(f"q:{m + 1}")
    P = sum(p[i] * x**i for i in range(n + 1))
    Q = sum(q[i] * x**i for i in range(m + 1)).subs(q[0], 1)

    expansion = sym.series(f, x, n=n + m + 1)
    eq = sym.expand(expansion * Q - P)
    coeffs = [eq.coeff(x, i) for i in range(n + m + 1)]
    res = sym.solve(coeffs)
    print(f"coeff: {coeffs}")
    if len(res) == 0:
        return None
    return (P / Q).subs(res)


def tri_poly(f: sym.Expr, interval: tuple, n: int, cont: bool, m: int = 0, raw: bool = False) -> sym.Expr:
    x = sym.symbols('x')
    k = sym.symbols('k')
    integ_range = list(interval)
    integ_range.insert(0, x)

    if cont == True:
        cos_term = sym.integrate(
            f * sym.cos(k * x), integ_range) * sym.cos(k * x)
        sin_term = sym.integrate(
            f * sym.sin(k * x), integ_range) * sym.sin(k * x)
        divider = sym.pi
    elif cont == False:
        j = sym.symbols('j')
        t = interval[0] + (interval[1] - interval[0]) / (2 * m) * j
        def trans(t): return sym.simplify((-sym.pi + 2 * sym.pi /
                                           (interval[1] - interval[0]) * (t - interval[0])))
        node = trans(t)
        cos_term = sym.Sum(f.subs(x, t) * sym.cos(k * node), (j, 0, 2 * m - 1)
                           ).doit() * (sym.cos(k * node).subs(node, trans(x)))
        sin_term = sym.Sum(f.subs(x, t) * sym.sin(k * node), (j, 0, 2 * m - 1)
                           ).doit() * (sym.sin(k * node).subs(node, trans(x)))
        divider = m
    sig = sym.Sum(cos_term + sin_term, (k, 1, n - 1)).doit()
    if m % n != 0:
        remained = cos_term.subs(k, 0) / 2 + cos_term.subs(k, n)
    elif m % n == 0:
        remained = cos_term.subs(k, 0) / 2 + cos_term.subs(k, n) / 2
    res = (sig + remained) / divider

    if raw == True:
        return res
    else:
        return res.evalf(7)


def FFT(y: np.ndarray) -> np.ndarray:
    N = len(y)

    if N == 1:
        return y
    else:
        c_even = FFT(y[::2])
        c_odd = FFT(y[1::2])
        factor = np.exp(2j * np.pi * np.arange(N) / N)
        c = np.concatenate([c_even + factor[:int(N / 2)] * c_odd,
                           c_even + factor[int(N / 2):] * c_odd])
        return c


def mk_result(t: list[float], w: list[float], exact_func: FunctionType) -> pd.DataFrame:
    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_func(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])
    return result
