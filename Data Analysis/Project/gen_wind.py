import numpy as np
import cv2


COLOR = {
    0: (0, 0, 0),
    1: (1, 1, 1),
    2: (2, 2, 2),
    3: (3, 3, 3),
    4: (4, 4, 4),
    5: (5, 5, 5),
    6: (6, 6, 6),
    7: (7, 7, 7),
    8: (8, 8, 8)
}

img = cv2.imread('images/map.png')
img = cv2.rectangle(img, (0, 0), (45, 114), COLOR[7], -1)
img = cv2.rectangle(img, (45, 0), (85, 114), COLOR[8], -1)
img = cv2.rectangle(img, (85, 0), (136, 114), COLOR[5], -1)
img = cv2.rectangle(img, (136, 0), (189, 114), COLOR[0], -1)
img = cv2.rectangle(img, (189, 0), (200, 114), COLOR[7], -1)
img = cv2.rectangle(img, (0, 114), (45, 300), COLOR[6], -1)
img = cv2.rectangle(img, (45, 114), (136, 198), COLOR[7], -1)
img = cv2.rectangle(img, (136, 114), (200, 300), COLOR[7], -1)
img = cv2.rectangle(img, (45, 198), (200, 300), COLOR[7], -1)

np.savetxt('txts\wind.txt', img[:, :, 0], fmt='%i')
print('saved!')
