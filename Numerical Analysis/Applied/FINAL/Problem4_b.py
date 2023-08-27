import os
import sympy as sym
import numpy as np
from plotly.subplots import make_subplots
import plotly.graph_objects as go
import time

from methods import tri_poly


def main():
    os.system("cls")
    x = sym.symbols('x')
    def func(x): return 2 / (5 - 4 * np.cos(x))
    expr = 2 / (5 - 4 * sym.cos(x))
    degs = [2, 4, 8, 16]  # M = N /2
    interval = (0, 2 * sym.pi)

    z = np.array([2 * np.pi / 256 * j for j in range(0, 257)])
    exact = func(z)
    fig = make_subplots(rows=2, cols=1)

    for deg in degs:
        start = time.time()
        tri_aprx, c = tri_poly(expr, interval, deg)  # direct calc
        print(f"Interpolate {expr} directly:")
        print(tri_aprx)
        print(f"Coefficient c_i for i = {-deg}, {-deg + 1}, ..., {deg - 1}:")
        print(f"{c[0]}, {c[1]} ..., {c[-1]}")
        # print(c)  # if you want to see all coefficient, then print this
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
