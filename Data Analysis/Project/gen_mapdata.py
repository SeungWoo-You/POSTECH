import os
import cv2
import numpy as np
import logging
import colorlog
from driectory import Directory as Dir


def scaling(pt: tuple[int, int]) -> tuple[int, int]:
    x = int(pt[0] * 2.5) + 5
    y = int(pt[1] * 2.1) - 7
    return (x, y)


def draw_spots(img: cv2.Mat, region: list[list[tuple[int]]]) -> cv2.Mat:
    for i, (pts, F) in enumerate(zip(region, FACTORIES)):
        done, fail = 0, 0
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
            if not np.array_equal(img[q][p], np.array([0, 0, 0])):
                cv2.line(img, (p, q), (p, q), COLOR)
                done += 1
            else:
                fail += 1
        logging.info(
            f"Region {i} with {F}: \033[35m{done}\033[0m done, {fail} deleted")
    return img


def main():
    region: list[list[tuple[int, int]]] = []
    for pts in POINTS:
        p1, p2 = scaling(pts[0]), scaling(pts[1])
        region.append([p1, p2])
    logging.info(f"Total \033[35m{len(region)} region\033[0m scaled")

    img_file = Dir.images / Dir.map
    img: cv2.Mat = cv2.imread(str(img_file))
    logging.info(f"Image \033[33m{img_file}\033[0m load")
    spotted = draw_spots(img.copy(), region)

    # danger code: may exist counter example
    np.putmask(spotted, spotted == [0, 0, 0], 0)
    np.putmask(spotted, spotted == [125, 125, 125], 2)
    np.putmask(spotted, ~(
        np.isin(spotted, [0, 0, 0]) | np.isin(spotted, [2, 2, 2])), 1)
    txt_file = Dir.files / Dir.map_data
    np.savetxt(txt_file, spotted[:, :, 0], fmt='%i')
    logging.info(f"File \033[33m{txt_file}\033[0m saved")


if __name__ == '__main__':
    os.system("cls")
    format = (
        "%(thin_purple)s%(asctime)s "
        "%(reset)s%(thin_blue)s%(bg_black)s%(funcName)s %(lineno)d "
        "%(reset)s%(log_color)s%(levelname)s: "
        "%(reset)s%(message)s"
    )
    colorlog.basicConfig(
        level=logging.INFO, format=format,
        datefmt="%m/%d/%Y %H:%M:%S"
    )
    POINTS: list[list[tuple[int, int]]] = [
        [(15, 27), (21, 33)],  # 파주
        [(18, 38), (44, 53)],  # 충청
        [(47, 71), (54, 81)],  # 대구
        [(44, 85), (66, 95)],  # 부산
        [(58, 75), (68, 85)],  # 울산
        [(18, 90), (25, 95)],  # 광주
        [(34, 93), (44, 98)]  # 경남
    ]
    FACTORIES: list[int] = [10, 60, 5, 30, 20, 5, 3]
    COLOR = (125, 125, 125)
    main()
