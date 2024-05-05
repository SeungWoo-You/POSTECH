import cv2
import dash_html_components as html
import dash_core_components as dcc
import dash
import plotly.express as px
import os

os.system('cls')
img = cv2.imread('images/map.png')
img = cv2.rectangle(img, (0, 0), (45, 114), (255, 0, 0), -1)
img = cv2.rectangle(img, (45, 0), (85, 114), (0, 255, 0), -1)
img = cv2.rectangle(img, (85, 0), (136, 114), (0, 0, 255), -1)
img = cv2.rectangle(img, (136, 0), (189, 114), (0, 255, 255), -1)
img = cv2.rectangle(img, (189, 0), (200, 114), (255, 0, 0), -1)
img = cv2.rectangle(img, (0, 114), (45, 300), (255, 255, 0), -1)
img = cv2.rectangle(img, (45, 114), (136, 198), (255, 0, 0), -1)
img = cv2.rectangle(img, (136, 114), (200, 300), (255, 255, 0), -1)
img = cv2.rectangle(img, (45, 198), (200, 300), (255, 255, 0), -1)
fig = px.imshow(img)
fig.update_layout(showlegend=False, height=600)
app = dash.Dash()
app.layout = html.Div([
    dcc.Graph(figure=fig)
]
)
app.run_server(debug=True, use_reloader=False)
