import os
from types import FunctionType
import numpy as np
import pandas as pd
import plotly.graph_objects as go

t_start: float = 1
t_end: float = 2
y_start: float = 0


def func(t: float, y: float) -> float:
    return 2 / t * y + t**2 * np.exp(t)


def exact_sol(t: float | pd.Series) -> float | pd.Series:
    return t**2 * (np.exp(t) - np.exp(1))


def Euler(t: float, w: float, h: float) -> float:
    return w + h * func(t, w)


def midpoint(t: float, w: float, h: float) -> float:
    m1 = w + h / 2 * func(t, w)
    return w + h * func(t + h / 2, m1)


def RK4(t: float, w: float, h: float) -> float:
    k1 = h * func(t, w)
    k2 = h * func(t + h / 2, w + 1 / 2 * k1)
    k3 = h * func(t + h / 2, w + 1 / 2 * k2)
    k4 = h * func(t + h, w + k3)
    return w + 1 / 6 * (k1 + 2 * k2 + 2 * k3 + k4)


def problem_a() -> None:
    t = [float(t_start)]
    w_Euler = [y_start]
    w_mid = [y_start]
    w_RK4 = [y_start]
    h = 0.1
    i = 0

    while np.isclose(t[i], t_end) == False:
        w_Euler.append(Euler(t[i], w_Euler[i], h))
        w_mid.append(midpoint(t[i], w_mid[i], h))
        w_RK4.append(RK4(t[i], w_RK4[i], h))
        t.append(t[i] + h)
        i += 1

    result = pd.DataFrame()
    result['t_i'] = t
    result['y(t_i)'] = exact_sol(result['t_i'])
    result['w_i: E'] = w_Euler
    result['e_i: E'] = np.abs(result['y(t_i)'] - result['w_i: E'])
    result['w_i: M'] = w_mid
    result['e_i: M'] = np.abs(result['y(t_i)'] - result['w_i: M'])
    result['w_i: RK'] = w_RK4
    result['e_i: RK'] = np.abs(result['y(t_i)'] - result['w_i: RK'])

    print("problem (a)")
    print(result, "\n")


def problem_b(nume_method: FunctionType, h: float) -> float:
    t = [float(t_start)]
    w = [y_start]
    i = 0

    while np.isclose(t[i], t_end) == False:
        w.append(nume_method(t[i], w[i], h))
        t.append(t[i] + h)
        i += 1

    return np.abs(exact_sol(t[-1]) - w[-1])


def main():
    h_list = [0.1, 0.05, 0.025]
    func_list = [Euler, midpoint, RK4]
    fig = go.Figure()

    os.system("cls")
    problem_a()

    for nume_method in func_list:
        error = []
        for h in h_list:
            error.append(problem_b(nume_method, h))

        fig.add_trace(
            go.Scatter(
                x=np.log(h_list),
                y=np.log(error),
                mode='markers+lines',
                name=f"method: {nume_method.__name__}",
                showlegend=True,
                hovertemplate='<br>'.join((
                    "log(h): %{x}",
                    "log(E): %{y}"
                )),
                hoverlabel=dict(namelength=0)
            )
        )

        slope = (np.log(error[2]) - np.log(error[0])) / \
            (np.log(h_list[2]) - np.log(h_list[0]))
        intercept = np.log(error[0]) - slope * np.log(h_list[0])

        fig.add_annotation(
            x=np.log(h_list[2]),
            y=np.log(error[2]) - 1,
            xanchor='left',
            text=f"log(E) = {slope:.3f}log(h) + {intercept:.3f}",
            showarrow=False,
            font_size=13
        )

    fig.update_layout(
        xaxis_title="log(h)",
        yaxis_title="log(E)",
        margin=dict(t=20, b=0)
    )

    fig.show()


if __name__ == "__main__":
    main()
