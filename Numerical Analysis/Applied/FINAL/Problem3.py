import os
import sympy as sym
import numpy as np
import plotly.graph_objects as go

from methods import LS_ortho


def problem_b() -> None:
    x = sym.symbols('x')
    f = sym.exp(-2 * x)
    fig = go.Figure()
    pts = np.arange(0, 10.1, 0.1)

    for i in range(1, 4):
        eq = LS_ortho(f, i)
        coef = np.array(eq.all_coeffs(), dtype=np.float64)
        print(f"coefficient of problem (b) with deg{i}: {coef}")
        val = np.array([eq.subs(x, t) for t in pts], dtype=np.float64)
        fig.add_trace(
            go.Scatter(
                x=pts,
                y=val,
                mode='lines',
                name=f"LS deg = {i}",
                showlegend=True
            ),
        )
        fig.add_annotation(
            x=2,
            y=0.5 + 0.4 * i,
            xanchor='left',
            text=f"poly = {eq.as_expr().evalf(5)}",
            showarrow=False,
            font_size=13,
        )

    val = np.array([f.subs(x, t) for t in pts], dtype=np.float64)
    fig.add_trace(
        go.Scatter(
            x=pts,
            y=val,
            mode='lines',
            name="exact f",
            showlegend=True
        ),
    )
    fig.update_layout(
        xaxis_title="x",
        yaxis_title="y",
        margin=dict(t=20, b=0)
    )
    fig.show()
    print("")


def main():
    os.system("cls")
    problem_b()


if __name__ == "__main__":
    main()
