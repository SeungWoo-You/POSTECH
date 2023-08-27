import numpy as np
import pandas as pd
from types import FunctionType


def idx(i: int) -> int:
    return -1 + i


def linear_shooting(bvp) -> pd.DataFrame:
    h = bvp.h
    N = bvp.N
    a, b = bvp.boundary_x[0], bvp.boundary_x[1]
    X = np.array([a + i * h for i in range(N)], dtype=np.float64)
    alpha, beta = bvp.boundary_y[0], bvp.boundary_y[1]
    u1, u2 = [alpha], [0]
    v1, v2 = [0], [1]
    p, q, r = bvp.given_p, bvp.given_q, bvp.given_r
    def r_0(x): return 0

    for x in X:
        res_u = get_k_linshoot(
            (p, q, r), x, h, (u1[-1], u2[-1]))
        res_v = get_k_linshoot(
            (p, q, r_0), x, h, (v1[-1], v2[-1]))
        u1.append(res_u[idx(1)])
        u2.append(res_u[idx(2)])
        v1.append(res_v[idx(1)])
        v2.append(res_v[idx(2)])

    k = (beta - u1[N]) / v1[N]
    w = []
    for i in range(N + 1):
        w.append(u1[i] + k * v1[i])

    X = np.append(X, a + N * h)
    res = pd.DataFrame()
    res['x'] = X
    res['y1'] = u1
    res['y2'] = v1
    res['nume_sol'] = w
    res['exact_sol'] = bvp.exact_sol(X)
    res['error'] = np.abs(res['exact_sol'] - res['nume_sol'])
    return res


def get_k_linshoot(funcs: tuple[FunctionType, FunctionType, FunctionType], x: float, h: float, nume: tuple[float, float]) -> tuple[float, float]:
    p, q, r = funcs
    k11 = h * nume[idx(2)]
    k12 = h * (p(x) * nume[idx(2)] + q(x) * nume[idx(1)] + r(x))
    k21 = h * (nume[idx(2)] + 1 / 2 * k12)
    k22 = h * (p(x + h / 2) * (nume[idx(2)] + 1 / 2 * k12) +
               q(x + h / 2) * (nume[idx(1)] + 1 / 2 * k11) + r(x + h / 2))
    k31 = h * (nume[idx(2)] + 1 / 2 * k22)
    k32 = h * (p(x + h / 2) * (nume[idx(2)] + 1 / 2 * k22) +
               q(x + h / 2) * (nume[idx(1)] + 1 / 2 * k21) + r(x + h / 2))
    k41 = h * (nume[idx(2)] + k32)
    k42 = h * (p(x + h) * (nume[idx(2)] + k32) +
               q(x + h) * (nume[idx(1)] + k31) + r(x + h))
    res1 = nume[idx(1)] + 1 / 6 * (k11 + 2 * k21 + 2 * k31 + k41)
    res2 = nume[idx(2)] + 1 / 6 * (k12 + 2 * k22 + 2 * k32 + k42)
    return (res1, res2)


def nonlin_shooting(bvp, iter_times: int = 1234, use_TOL: bool = True) -> pd.DataFrame:
    h = bvp.h
    N = bvp.N
    a, b = bvp.boundary_x[0], bvp.boundary_x[1]
    X = np.array([a + i * h for i in range(N)], dtype=np.float64)
    alpha, beta = bvp.boundary_y[0], bvp.boundary_y[1]
    w1 = [alpha]
    f, fy, fyp = bvp.given_f, bvp.given_fy, bvp.given_fyp

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


def linfin_difference(bvp) -> pd.DataFrame:
    h = bvp.h
    N = bvp.N - 1
    a, b = bvp.boundary_x[0], bvp.boundary_x[1]
    X = np.array([a + i * h for i in range(1, N + 1)], dtype=np.float64)
    alpha, beta = bvp.boundary_y[0], bvp.boundary_y[1]
    p, q, r = bvp.given_p, bvp.given_q, bvp.given_r

    A = [None]
    B = [None]
    C = [None]
    D = [None]
    for x in X:
        A.append(2 + h**2 * q(x))
        B.append(-1 + h / 2 * p(x))
        C.append(-1 - h / 2 * p(x))
        D.append(-h**2 * r(x))

    D[1] = -h**2 * r(X[idx(1)]) - C[1] * alpha
    D[N] = -h**2 * r(X[idx(N)]) - B[N] * beta

    l = [None]
    u = [0]
    z = [0]
    for i in range(1, N + 1):
        l.append(A[i] - C[i] * u[i - 1])
        u.append(B[i] / l[i])
        z.append((D[i] - C[i] * z[i - 1]) / l[i])
    C[1] = None
    B[N] = None
    u[N] = None
    u[0] = None
    z[0] = None

    w = [alpha, z[N], beta]
    for i in range(1, N):
        w.insert(1, z[N - i] - u[N - i] * w[1])

    X = np.append(X, a + (N + 1) * h)
    X = np.insert(X, 0, a)
    res = pd.DataFrame()
    res['x'] = X
    res['nume_sol'] = w
    res['exact_sol'] = bvp.exact_sol(X)
    res['error'] = np.abs(res['exact_sol'] - res['nume_sol'])
    return res


def extrapol(w1: np.ndarray, w2: np.ndarray, iters: int) -> np.ndarray:
    return w2 - (w2 - w1) / (4**iters - 1)


def nonlinfin_difference(bvp, iter_times: int = 1234, use_TOL: bool = True) -> pd.DataFrame:
    h = bvp.h
    N = bvp.N - 1
    a, b = bvp.boundary_x[0], bvp.boundary_x[1]
    alpha, beta = bvp.boundary_y[0], bvp.boundary_y[1]
    f, fy, fyp = bvp.given_f, bvp.given_fy, bvp.given_fyp

    w = [alpha + i * (beta - alpha) / (b - a) * h for i in range(N + 2)]
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
            if np.linalg.norm(v) <= bvp.TOL:
                break

    res = pd.DataFrame()
    x.append(b)
    res['x'] = x
    res['nume_sol'] = w
    res['exact_sol'] = bvp.exact_sol(np.array(x))
    res['error'] = np.abs(res['exact_sol'] - res['nume_sol'])
    return res
