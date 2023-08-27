import os
import sys
import numpy as np
import pandas as pd
from types import FunctionType
import sympy as sym
import plotly.graph_objects as go

from methods import AB4, AM3


def func(t: float, y: float | sym.Expr) -> float | sym.Expr:
    return y - t**2 + 1


def exact_sol(t: float | np.ndarray) -> float | np.ndarray:
    return (t + 1)**2 - 0.5 * np.exp(t)


def mk_table(df1: pd.DataFrame, df2: pd.DataFrame, method1: str, method2: str) -> pd.DataFrame:
    df2_mod = df2.drop(labels=['t_i', 'exact'], axis=1)
    df2_mod.rename(columns={'w_i': f'w_i: {method2}',
                   'error': f'error: {method2}'}, inplace=True)
    df1_mod = df1[['t_i', 'exact', 'w_i', 'error']]
    df = pd.concat([df1_mod, df2_mod], axis=1)
    df.rename(columns={'w_i': f'w_i: {method1}',
              'error': f'error: {method1}'}, inplace=True)
    return df


def mk_result(t: list[float], w: list[float], exact_func: FunctionType) -> pd.DataFrame:
    result = pd.DataFrame()
    result['t_i'] = t
    result['w_i'] = w
    result['exact'] = exact_func(result['t_i'])
    result['error'] = np.abs(result['exact'] - result['w_i'])
    return result


def problem_a(step: int, method: FunctionType) -> pd.DataFrame:
    t_start = 0.0
    t_end: float = 2.0
    h: float = 0.1
    t = [t_start + h * i for i in range(step)]
    w = list(exact_sol(np.array(t)))

    while t[-1] + sys.float_info.epsilon < t_end:
        w.append(method(t, w, h, func))
        t.append(t[-1] + h)
    result = mk_result(t, w, exact_sol)
    return result


def problem_b(step: int, method: FunctionType, h_list: list) -> list[float]:
    t_start = 0.0
    t_end: float = 2.0
    res = []

    for h in h_list:
        t = [t_start + h * i for i in range(step)]
        w = list(exact_sol(np.array(t)))
        while t[-1] + sys.float_info.epsilon < t_end:
            w.append(method(t, w, h, func))
            t.append(t[-1] + h)
        res.append(abs(exact_sol(t[-1]) - w[-1]))
    return res


def main():
    h_list = [0.1, 0.05, 0.025]
    scheme_dict = {AB4: 4, AM3: 3}

    os.system("cls")
    print("problem (c)")
    # problem (a)
    temp = []
    for method, step in scheme_dict.items():
        temp.append(problem_a(step, method))
    res = mk_table(temp[0], temp[1], AB4.__name__, AM3.__name__)
    print(res)
    print("")

    # problem (b)
    fig = go.Figure()
    for method, step in scheme_dict.items():
        err = problem_b(step, method, h_list)
        fig.add_trace(
            go.Scatter(
                x=np.log(h_list),
                y=np.log(err),
                mode='markers+lines',
                name=f"method: {method.__name__}",
                showlegend=True,
                hovertemplate='<br>'.join((
                    "log(h): %{x}",
                    "log(E): %{y}"
                )),
                hoverlabel=dict(namelength=0)
            ),
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
            font_size=13,
        )
        print(f"{method.__name__}")
        print(f"h: {h_list}")
        print(f"err: {err}")

    fig.update_layout(
        xaxis_title="log(h)",
        yaxis_title="log(E)",
        margin=dict(t=20, b=0)
    )
    fig.show()


if __name__ == "__main__":
    main()
