import os
import numpy as np
import pandas as pd
import plotly.graph_objects as go

import methods


def func(t: float, y: float) -> float:
    return y - t**2 + 1


def exact_sol(t: float | np.ndarray) -> float | np.ndarray:
    return (t + 1)**2 - 0.5 * np.exp(t)


def problem_a() -> None:
    h = 0.1
    t_start = 0.0
    t_end = 2.0
    y_start = 0.5
    N = int((t_end - t_start) / h)
    t = [t_start + i * h for i in range(N + 1)]
    mets = [methods.Euler, methods.midpoint, methods.RK4]

    res = pd.DataFrame()
    res['t_i'] = t
    res['exact'] = exact_sol(res['t_i'])
    del t[-1]

    for met in mets:
        w = [y_start]
        for t_i in t:
            w_next = met(t_i, w[-1], h, func)
            w.append(w_next)
        res[f'w_{met.__name__}'] = w
        res[f'e_{met.__name__}'] = res['exact'] - res[f'w_{met.__name__}']

    cols = ['t_i', 'exact']
    for met in mets:
        cols.append(f'w_{met.__name__}')
        cols.append(f'e_{met.__name__}')
    print("problem (a)")
    res = res[cols]
    print(res)
    print("")
    return


def problem_b() -> None:
    h_list = [0.1, 0.05, 0.025]
    t_start = 0.0
    t_end = 2.0
    y_start = 0.5
    N = [int((t_end - t_start) / h) for h in h_list]
    mets = [methods.Euler, methods.midpoint, methods.RK4]

    res = pd.DataFrame()
    fig = go.Figure()
    res['h'] = h_list

    for met in mets:
        err = []
        for h, n in zip(h_list, N):
            t = [t_start + i * h for i in range(n)]
            w = [y_start]
            for t_i in t:
                w_next = met(t_i, w[-1], h, func)
                w.append(w_next)
            truth = exact_sol(t[-1] + h)
            err.append(np.abs(truth - w[-1]))
        res[f'e_{met.__name__}'] = err

        fig.add_trace(
            go.Scatter(
                x=np.log(res['h']),
                y=np.log(res[f'e_{met.__name__}']),
                mode='markers+lines',
                name=f"method: {met.__name__}",
                showlegend=True,
                hovertemplate='<br>'.join((
                    "log(h): %{x}",
                    "log(E): %{y}"
                )),
                hoverlabel=dict(namelength=0)
            )
        )

        slope = (np.log(err[2]) - np.log(err[0])) / \
            (np.log(h_list[2]) - np.log(h_list[0]))
        intercept = np.log(err[0]) - slope * np.log(h_list[0])

        fig.add_annotation(
            x=np.log(h_list[2]),
            y=np.log(err[2]) - 1,
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

    print("problem (b)")
    print(res)
    print("")


def main():
    os.system("cls")
    problem_a()
    problem_b()


if __name__ == "__main__":
    main()
