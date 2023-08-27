import os
import numpy as np
import sympy as sym
import plotly.graph_objects as go
from types import FunctionType

from methods import tri_poly


def main():
    os.system("cls")
    x = sym.symbols('x')
    def f_b(x): return x * (np.pi - x)
    def f_c(x): return np.abs(x)
    f_dict: dict[FunctionType, sym.Expr] = {
        f_b: x * (sym.pi - x), f_c: sym.Abs(x)}
    deg = 2
    interval = (-sym.pi, sym.pi)

    t = np.linspace(np.float64(interval[0].evalf()), np.float64(
        interval[1].evalf()), 50)
    fig = go.Figure()

    for func, expr in f_dict.items():
        tri_aprx = tri_poly(expr, interval, deg,
                            cont=False, m=deg)  # direct calc
        print(f"interpolate {func.__name__} = {expr}")
        print(tri_aprx)
        print("")
        w = [np.float64(tri_aprx.subs(x, t_i)) for t_i in t]
        y = func(t)
        fig.add_trace(
            go.Scatter(
                x=t,
                y=y - w,
                mode='lines',
                name=f"error of {expr}",
                showlegend=True,
                hoverinfo='skip'
            ),
        )
    fig.update_layout(
        xaxis_title="x",
        yaxis_title="y",
        margin=dict(t=20, b=0)
    )
    fig.show()


if __name__ == "__main__":
    main()
