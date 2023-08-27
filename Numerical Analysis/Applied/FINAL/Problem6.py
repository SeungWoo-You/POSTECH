import os
import numpy as np
import pandas as pd
from plotly.subplots import make_subplots
import plotly.graph_objects as go

from methods import nonlinfin_difference


class BVP:
    def __init__(self, h: float, boundary_x: tuple[float, float], boundary_y: tuple[float, float]) -> None:
        self.h = h
        self.boundary_x = boundary_x
        self.boundary_y = boundary_y
        self.N = int((boundary_x[1] - boundary_x[0]) / h)
        self.TOL = 10**(-8)

    def given_f(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return -yp**2

    def given_fy(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return 0

    def given_fyp(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return -2 * yp

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        return np.log((np.e - 1) * x + 1)


def main():
    os.system("cls")
    h = 0.1
    bvp = BVP(h, (0, 1), (0, 1))
    N = [2**i for i in range(3, 11)]
    err = []
    fig = make_subplots(rows=2, cols=1)

    for n in N:
        h = 1 / n
        bvp = BVP(h, (0, 1), (0, 1))
        res = nonlinfin_difference(bvp, use_TOL=True)
        error = np.linalg.norm(res['error'])
        err.append(error)
        if n == 2**3:
            fig.add_trace(
                go.Scatter(
                    x=res['x'],
                    y=res['nume_sol'],
                    mode='lines',
                    name=f"U with N = {n}",
                    showlegend=True
                ),
                row=2, col=1
            )

    data = zip(N, err)
    table = pd.DataFrame(data)
    table.columns = ['N', 'error']
    print(table)

    fig.add_trace(
        go.Scatter(
            x=1 / table['N'],
            y=table['error'],
            mode='markers+lines',
            name=f"h vs error",
            showlegend=True,
            hovertemplate='<br>'.join((
                "h: %{x}",
                "error: %{y}"
            )),
            hoverlabel=dict(namelength=0)
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
