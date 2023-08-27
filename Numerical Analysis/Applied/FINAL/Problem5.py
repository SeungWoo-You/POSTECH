import os
import numpy as np
import sympy as sym
from methods import nonlin_shooting
import plotly.graph_objects as go


class BVP:
    def __init__(self, h: float, boundary_x: tuple[float, float], boundary_y: tuple[float, float], lam: float) -> None:
        self.h = h
        self.boundary_x = boundary_x
        self.boundary_y = boundary_y
        self.N = int((boundary_x[1] - boundary_x[0]) / h)
        self.TOL = 10**(-5)
        self.lam = lam

    def given_f(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return -self.lam * np.exp(y)

    def given_fy(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return -self.lam * np.exp(y)

    def given_fyp(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return 0

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        O = sym.symbols('O')
        expr = sym.sqrt(2 * self.lam) * sym.cosh(O / 4) - O
        r = np.float64(sym.nsolve(expr, False))

        in_term = np.cosh((x - 1 / 2) * r / 2) / np.cosh(r / 4)
        return -2 * np.log(in_term)


def main():
    os.system("cls")
    h = 0.2
    bvp = BVP(h, (0, 1), (0, 0), 1)
    bvp_g = BVP(0.02, (0, 1), (0, 0), 1)
    S = [1, 15]
    fig = go.Figure()

    for s in S:
        res = nonlin_shooting(bvp, TK=s, use_TOL=False)
        print(f"Nonlinear Shooting with s = {s}")
        print(res)
        print("")
        res = nonlin_shooting(bvp_g, TK=s, use_TOL=False)
        fig.add_trace(
            go.Scatter(
                x=res['x'],
                y=res['nume_sol'],
                mode='lines',
                name=f"numerical with s = {s}",
                showlegend=True
            )
        )
    fig.add_trace(
        go.Scatter(
            x=res['x'],
            y=res["exact_sol"],
            mode='lines',
            name=f"exact",
            showlegend=True
        )
    )
    fig.update_layout(
        xaxis_title="x",
        yaxis_title="y",
        margin=dict(t=20, b=0)
    )
    fig.show()


if __name__ == "__main__":
    main()
