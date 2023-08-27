import os
import sympy as sym
import numpy as np
from plotly.subplots import make_subplots
import plotly.graph_objects as go
import time
from types import FunctionType

from methods import FFT


def problem_c(func: FunctionType, interval: tuple, m: int) -> tuple:
    x = np.array([interval[0] + (interval[1] - interval[0]) /
                 (2 * m) * j for j in range(2 * m)])
    x = np.interp(x, interval, (-np.pi, np.pi))
    y = func(x)
    cal = FFT(y)[:m + 1]

    a = cal.real / m
    np.put(a, [0, -1], [a[0] / 2, a[-1] / 2])
    b = cal.imag / m
    c_plus = [(a[ii] - 1j * b[ii - 1]) / 2 for ii in range(1, m)]
    c = [(a[ii] + 1j * b[ii - 1]) / 2 for ii in range(1, m)[::-1]]
    c.insert(0, a[-1])  # already divided by 2
    c.append(a[0])  # already divided by 2
    c.extend(c_plus)
    for i in range(m + 1):
        a[i] = a[i] * (-1)**i
        try:
            b[i] = b[i] * (-1)**i
        except:
            pass
    b = np.delete(b, [0, -1])

    t = sym.symbols('t')
    cos_term = [a[i] * sym.cos(i * t) for i in range(1, m)]
    remained = a[0] + a[-1] * sym.cos(m * t)
    sin_term = [b[i - 1] * sym.sin(i * t) for i in range(1, m)]

    expr = remained
    for cos_, sin_ in zip(cos_term, sin_term):
        expr += cos_ + sin_

    return (expr.evalf(7), c)


def main():
    os.system("cls")
    x = sym.symbols('x')
    def func(x): return 2 / (5 - 4 * np.cos(x))
    expr = 2 / (5 - 4 * sym.cos(x))
    degs = [2, 4, 8, 16]  # M = N /2
    interval = (0, 2 * np.pi)

    z = np.array([2 * np.pi / 256 * j for j in range(0, 257)])
    exact = func(z)
    fig = make_subplots(rows=2, cols=1)

    for deg in degs:

        start = time.time()
        print("use FFT")
        tri_aprx, c = problem_c(func, interval, deg)  # use FFT
        tri_aprx = tri_aprx.subs('t', x)
        print(f"Interpolate {expr} FFT:")
        print(tri_aprx)
        print(
            f"Coefficient c_i for i = {-deg}, {-deg + 1}, ..., {deg - 1}:")
        print(f"{c[0]}, {c[1]} ..., {c[-1]}")
        # print(c) # if you want to see all coefficient, then print this

        end = time.time()
        print(f"calc time: {end - start}")
        print("")

        U = np.array([tri_aprx.subs(x, z_i) for z_i in z], dtype=np.float64)
        err = np.abs(exact - U)
        fig.add_trace(
            go.Scatter(
                x=z,
                y=U,
                mode='lines',
                name=f"U with N = {2 * deg}",
                showlegend=True
            ),
            row=1, col=1
        )
        fig.add_trace(
            go.Scatter(
                x=z,
                y=err,
                mode='lines',
                name=f"err with N = {2 * deg}",
                showlegend=True
            ),
            row=2, col=1
        )

    fig.add_trace(
        go.Scatter(
            x=z,
            y=exact,
            mode='lines',
            name="exact",
            showlegend=True
        ),
        row=1, col=1
    )
    fig.update_layout(
        xaxis_title="x",
        yaxis_title="y",
        margin=dict(t=20, b=0)
    )
    fig.show()


if __name__ == "__main__":
    main()
