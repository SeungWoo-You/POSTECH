from simulation import Simulation
from dash import Dash, dcc, html, Input, Output
import plotly.graph_objects as go
import numpy as np
import os
import signal

# os.system('cls')
if __name__ == '__main__':
    print('run')
    sim = Simulation()
    print('simulation created')
    app = Dash(__name__)
    app.layout = html.Div([
        html.H4(children='PM-2.5 Concentration at 0', id='title'),
        dcc.Graph(
            id='graph',
            figure=go.Figure(
                data=go.Heatmap(
                    z=sim.afterprocess(),
                    type='heatmap',
                    colorscale='reds',
                    hovertemplate="amount %{z}",
                    hoverlabel=dict(namelength=0)
                ),
            )
        ),
        dcc.Interval(
            id='interval',
            interval=0.8 * 1000,  # in milliseconds
            n_intervals=0  # 초기 값 확인 필요
        )
    ])

    @app.callback(  # long-callback 써야할 수 있음
        Output('title', 'children'),
        Output('graph', 'figure'),
        Input('interval', 'n_intervals')
    )
    def update(n: int):
        print(n)
        if n == 100000:
            os.kill(os.getpid(), signal.SIGTERM)
        sim.run(n)
        fig = go.Figure(
            data=go.Heatmap(
                z=sim.afterprocess(),
                type='heatmap',
                colorscale='rainbow',
                hovertemplate="amount %{z}",
                hoverlabel=dict(namelength=0),
            ),
        )
        fig.update_layout(width=400, height=600)
        fig.update_yaxes(autorange="reversed")
        return (f'PM-2.5 Concentration at {n}', fig)

    app.run_server(debug=True)
