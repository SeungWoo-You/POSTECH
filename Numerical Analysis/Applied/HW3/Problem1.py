import os
import numpy as np
import plotly.graph_objects as go

from methods import LS_poly, LSE


def main():
    os.system("cls")
    x = np.array([0, 0.15, 0.31, 0.5, 0.6, 0.75])
    y = np.array([1.0, 1.004, 1.031, 1.117, 1.223, 1.422])
    fig = go.Figure()

    for deg in range(1, 4):
        A = LS_poly(x, y, deg, cont=False)  # same as np.polyfit(x, y, deg)
        poly = np.poly1d(A)
        poly_x = np.linspace(x[0], x[-1], 50)
        poly_y = [poly(i) for i in poly_x]
        fig.add_trace(
            go.Scatter(
                x=poly_x,
                y=poly_y,
                mode='lines',
                name=f"polynomial of degrees {deg}",
                showlegend=True,
                hoverinfo='skip'
            ),
        )
        fig.add_annotation(
            x=x[0],
            y=1 + 0.1 * deg,
            xanchor='left',
            text=f"coef of deg {deg}: {A}<br>LSE: {LSE(x, y, poly)}",
            showarrow=False,
            font_size=13,
        )

    fig.add_trace(
        go.Scatter(
            x=x,
            y=y,
            mode='markers',
            showlegend=False,
            hovertemplate='<br>'.join((
                "x: %{x}",
                "y: %{y}"
            )),
            hoverlabel=dict(namelength=0)
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
