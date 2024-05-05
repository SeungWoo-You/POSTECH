import cv2
import numpy as np
import dash_html_components as html
import dash_core_components as dcc
import dash
import plotly.express as px
import os

os.system('cls')


def find_pts(img):
    points: list[list[tuple[int, int]]] = [
        [(153, 275), (214, 331)],  # 파주
        [(181, 380), (448, 538)],  # 충청
        [(470, 710), (547, 814)],  # 대구
        [(441, 850), (667, 957)],  # 부산
        [(580, 755), (687, 850)],  # 울산
        [(182, 904), (251, 959)],  # 광주
        [(342, 931), (444, 981)]  # 경남
    ]
    res = []
    red = (255, 0, 0)
    """for pt in points:
        p1, p2 = pt
        cv2.rectangle(img, scaling(p1), scaling(p2), red, 10)"""
    for pts in points:
        p1, p2 = scaling(pts[0]), scaling(pts[1])
        res.append([p1, p2])
    return res, img


def scaling(pt: tuple[int, int]) -> tuple[int, int]:
    x = int(pt[0] * 2.5) + 50
    y = int(pt[1] * 2.1) - 70
    return (x, y)


img = cv2.imread('images/map.png')
L, img = find_pts(img)
factories = [100, 600, 50, 300, 200, 50, 30]
for i, (pts, F) in enumerate(zip(L, factories)):
    p1, p2 = pts
    x1, y1, = p1
    x2, y2 = p2
    Dx, Dy = x2 - x1, y2 - y1
    x_scale, y_scale = np.random.normal(size=F), np.random.normal(size=F)
    x_scale = (x_scale - min(x_scale)) / (max(x_scale) - min(x_scale))
    y_scale = (y_scale - min(y_scale)) / (max(y_scale) - min(y_scale))
    for sx, sy in zip(x_scale, y_scale):
        dx, dy = int(sx * Dx), int(sy * Dy)
        p, q = x1 + dx, y1 + dy
        if np.array_equal(img[q][p], np.array([204, 204, 205])):
            cv2.line(img, (p, q), (p, q), (255, 0, 0), 10)

fig = px.imshow(img)
fig.update_layout(showlegend=False, height=600)
app = dash.Dash()
app.layout = html.Div([
    dcc.Graph(figure=fig)
]
)
app.run_server(debug=True, use_reloader=False)
cv2.imwrite(str('images/factory_map_dots.png'), img)
