import numpy as np
import pandas as pd
import sympy as sym
from types import FunctionType


def Euler(t: float, w: float, h: float, f: FunctionType) -> float:
    return w + h * f(t, w)


def midpoint(t: float, w: float, h: float, f: FunctionType) -> float:
    m1 = w + h / 2 * f(t, w)
    return w + h * f(t + h / 2, m1)


def RK4(t: float, w: float, h: float, f: FunctionType) -> float:
    k1 = h * f(t, w)
    k2 = h * f(t + h / 2, w + 1 / 2 * k1)
    k3 = h * f(t + h / 2, w + 1 / 2 * k2)
    k4 = h * f(t + h, w + k3)
    return w + 1 / 6 * (k1 + 2 * k2 + 2 * k3 + k4)


def AB4(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    return w[-1] + h / 24 * (55 * f(t[-1], w[-1]) - 59 * f(t[-2], w[-2]) + 37 * f(t[-3], w[-3]) - 9 * f(t[-4], w[-4]))


def AM3(t: list[float], w: list[float], h: float, f: FunctionType) -> float:
    W = sym.Symbol('x')
    equation = sym.Eq(W, w[-1] + h / 24 * (9 * f(t[-1] + h, W) + 19 *
                      f(t[-1], w[-1]) - 5 * f(t[-2], w[-2]) + f(t[-3], w[-3])))
    return np.float64(sym.solve(equation)[0])


def LS_ortho(f: sym.Expr, n: int) -> sym.Poly:
    X = np.empty((0, n + 1), dtype=np.float64)
    Y = np.array([])
    x = sym.symbols('x')
    L0 = 1
    L1 = x - 1
    L2 = x**2 - 4 * x + 2
    L3 = x**3 - 9 * x**2 + 18 * x - 6
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
    return eq


def tri_poly(f: sym.Expr, interval: tuple, m: int, raw: bool = False) -> tuple[sym.Expr, list]:
    x = sym.symbols('x')
    k = sym.symbols('k')
    j = sym.symbols('j')
    t = interval[0] + (interval[1] - interval[0]) / (2 * m) * j

    def trans(t): return sym.simplify((-sym.pi + 2 * sym.pi /
                                       (interval[1] - interval[0]) * (t - interval[0])))
    node = trans(t)
    a = sym.Sum(f.subs(x, t) * sym.cos(k * node), (j, 0, 2 * m - 1)).doit() / m
    b = sym.Sum(f.subs(x, t) * sym.sin(k * node), (j, 0, 2 * m - 1)).doit() / m
    cos_term = a * (sym.cos(k * node).subs(node, trans(x)))
    sin_term = b * (sym.sin(k * node).subs(node, trans(x)))
    sig = sym.Sum(cos_term + sin_term, (k, 1, m - 1)).doit()
    remained = cos_term.subs(k, 0) / 2 + cos_term.subs(k, m) / 2
    res = (sig + remained)

    c_plus = [((a.subs(k, ii) - sym.I * b.subs(k, ii)) / 2).evalf(5)
              for ii in range(1, m)]
    c = [((a.subs(k, ii) + sym.I * b.subs(k, ii)) / 2).evalf(5)
         for ii in range(1, m)[::-1]]
    c.insert(0, (a.subs(k, m) / 2).evalf(5))
    c.append((a.subs(k, 0) / 2).evalf(5))
    c.extend(c_plus)

    if raw == True:
        return (res, c)
    else:
        return (res.evalf(7), c)


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


def idx(i: int) -> int:
    return -1 + i


def nonlin_shooting(bvp, TK: float = None, iter_times: int = 1234, use_TOL: bool = True) -> pd.DataFrame:
    h = bvp.h
    N = bvp.N
    a, b = bvp.boundary_x[0], bvp.boundary_x[1]
    X = np.array([a + i * h for i in range(N)], dtype=np.float64)
    alpha, beta = bvp.boundary_y[0], bvp.boundary_y[1]
    w1 = [alpha]
    f, fy, fyp = bvp.given_f, bvp.given_fy, bvp.given_fyp

    if TK == None:
        TK = (beta - alpha) / (b - a)
    for i in range(iter_times):
        w1 = [alpha]
        w2 = [TK]
        u1 = 0
        u2 = 1
        for x in X:
            res_uv = get_k_nonlinshoot(
                (f, fy, fyp), x, h, (w1[-1], w2[-1]), (u1, u2))
            w1.append(res_uv[idx(1)])
            w2.append(res_uv[idx(2)])
            u1 = res_uv[idx(3)]
            u2 = res_uv[idx(4)]
        TK = TK - (w1[-1] - beta) / u1

        if use_TOL == True:
            if np.abs(w1[-1] - beta) <= bvp.TOL:
                break

    X = np.append(X, a + N * h)
    res = pd.DataFrame()
    res['x'] = X
    res['nume_sol'] = w1
    res['exact_sol'] = bvp.exact_sol(X)
    res['error'] = np.abs(res['exact_sol'] - res['nume_sol'])
    return res


def get_k_nonlinshoot(funcs: tuple[FunctionType, FunctionType, FunctionType], x: float, h: float, nume: tuple[float, float], u: tuple[float, float]) -> tuple[float, float, float, float]:
    f, fy, fyp = funcs
    k11 = h * nume[idx(2)]
    k12 = h * f(x, nume[idx(1)], nume[idx(2)])
    k21 = h * (nume[idx(2)] + 1 / 2 * k12)
    k22 = h * f(x + h / 2, nume[idx(1)] + 1 / 2 *
                k11, nume[idx(2)] + 1 / 2 * k12)
    k31 = h * (nume[idx(2)] + 1 / 2 * k22)
    k32 = h * f(x + h / 2, nume[idx(1)] + 1 / 2 *
                k21, nume[idx(2)] + 1 / 2 * k22)
    k41 = h * (nume[idx(2)] + k32)
    k42 = h * f(x + h, nume[idx(1)] + k31, nume[idx(2)] + k32)
    res1 = nume[idx(1)] + 1 / 6 * (k11 + 2 * k21 + 2 * k31 + k41)
    res2 = nume[idx(2)] + 1 / 6 * (k12 + 2 * k22 + 2 * k32 + k42)

    k11 = h * u[idx(2)]
    k12 = h * (fy(x, nume[idx(1)], nume[idx(2)]) * u[idx(1)] +
               fyp(x, nume[idx(1)], nume[idx(2)]) * u[idx(2)])
    k21 = h * (u[idx(2)] + 1 / 2 * k12)
    k22 = h * (fy(x + h / 2, nume[idx(1)], nume[idx(2)]) * (u[idx(1)] + 1 / 2 * k11) + fyp(
        x + h / 2, nume[idx(1)], nume[idx(2)]) * (u[idx(2)] + 1 / 2 * k12))
    k31 = h * (u[idx(2)] + 1 / 2 * k22)
    k32 = h * (fy(x + h / 2, nume[idx(1)], nume[idx(2)]) * (u[idx(1)] + 1 / 2 * k21) + fyp(
        x + h / 2, nume[idx(1)], nume[idx(2)]) * (u[idx(2)] + 1 / 2 * k22))
    k41 = h * (u[idx(2)] + k32)
    k42 = h * (fy(x + h, nume[idx(1)], nume[idx(2)]) * (u[idx(1)] + k21) +
               fyp(x + h, nume[idx(1)], nume[idx(2)]) * (u[idx(2)] + k22))
    res3 = u[idx(1)] + 1 / 6 * (k11 + 2 * k21 + 2 * k31 + k41)
    res4 = u[idx(2)] + 1 / 6 * (k12 + 2 * k22 + 2 * k32 + k42)
    return (res1, res2, res3, res4)


def nonlinfin_difference(bvp, iter_times: int = 1234, use_TOL: bool = True) -> pd.DataFrame:
    h = bvp.h
    N = bvp.N - 1
    a, b = bvp.boundary_x[0], bvp.boundary_x[1]
    alpha, beta = bvp.boundary_y[0], bvp.boundary_y[1]
    f, fy, fyp = bvp.given_f, bvp.given_fy, bvp.given_fyp

    w = [alpha + i * (beta - alpha) / (b - a) * h for i in range(N + 2)]
    w0 = w.copy()
    for j in range(iter_times):
        x = [a]
        A = [None]
        B = [None]
        C = [None]
        D = [None]
        for i in range(1, N + 1):
            x.append(a + i * h)
            t = (w[i + 1] - w[i - 1]) / (2 * h)
            A.append(2 + h**2 * fy(x[i], w[i], t))
            B.append(-1 + h / 2 * fyp(x[i], w[i], t))
            C.append(-1 - h / 2 * fyp(x[i], w[i], t))
            D.append(-(2 * w[i] - w[i + 1] -
                     w[i - 1] + h**2 * f(x[i], w[i], t)))

        l = [None]
        u = [0]
        z = [0]
        for i in range(1, N + 1):
            l.append(A[i] - C[i] * u[i - 1])
            u.append(B[i] / l[i])
            z.append((D[i] - C[i] * z[i - 1]) / l[i])
        C[1] = None
        B[N] = None
        u[N] = 0
        u[0] = None
        z[0] = None

        v = [0 for i in range(N + 2)]
        for i in range(1, N + 1)[::-1]:
            v[i] = z[i] - u[i] * v[i + 1]
            w[i] += v[i]

        if use_TOL == True:
            if np.linalg.norm(v) < bvp.TOL * (np.linalg.norm(w0) + 1):
                break

    res = pd.DataFrame()
    x.append(b)
    res['x'] = x
    res['nume_sol'] = w
    res['exact_sol'] = bvp.exact_sol(np.array(x))
    res['error'] = np.abs(res['exact_sol'] - res['nume_sol'])
    return res
