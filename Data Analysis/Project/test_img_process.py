import dash_html_components as html
import dash_core_components as dcc
import dash
import cv2
import os
from pathlib import Path
import plotly.graph_objs as go
import plotly.express as px


def draw_region(img):
    points: list[list[tuple[int, int]]] = [
        [(15, 27), (21, 33)],  # 파주
        [(18, 38), (44, 53)],  # 충청
        [(47, 71), (54, 81)],  # 대구
        [(44, 85), (66, 95)],  # 부산
        [(58, 75), (68, 85)],  # 울산
        [(18, 90), (25, 95)],  # 광주
        [(34, 93), (44, 98)]  # [(312, 841), (444, 911)]  # 경남
    ]
    red = (255, 0, 0)
    for pt in points:
        p1, p2 = pt
        cv2.rectangle(img, scaling(p1), scaling(p2), red, 1)
    return img


def scaling(pt: tuple[int, int]) -> tuple[int, int]:
    x = int(pt[0] * 2.5) + 5
    y = int(pt[1] * 2.1) - 7
    return (x, y)


def main():
    img_file = IMG / 'map.png'
    img = cv2.imread(str(img_file))
    img = draw_region(img)
    fig = px.imshow(img)
    fig.update_layout(showlegend=False, height=600)
    app = dash.Dash()
    app.layout = html.Div([
        dcc.Graph(figure=fig)
    ]
    )
    app.run_server(debug=True, use_reloader=False)


if __name__ == '__main__':
    os.system('cls')
    IMG = Path('images')
    main()
