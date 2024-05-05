import os
import cv2
import numpy as np
import logging
import colorlog
from driectory import Directory as Dir


def main():
    img_file = Dir.images / Dir.dust_map
    img: cv2.Mat = cv2.imread(str(img_file))
    logging.info(f"Image \033[33m{img_file}\033[0m load")
    spotted = np.zeros((300, 200))

    # image 좌표에 scale 해주자. 원하는 txt 좌표와 img 파일의 크기가 달라서 해당 과정이 필요하다.
    for i in range(300):
        for j in range(200):
            if all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [255, 255, 255]):
                spotted[i][j] = 0
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [152, 51, 51]):
                spotted[i][j] = 2
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [191, 51, 51]):
                spotted[i][j] = 5
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [230, 51, 51]):
                spotted[i][j] = 10
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [255, 63, 51]):
                spotted[i][j] = 15
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [255, 102, 51]):
                spotted[i][j] = 20
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [255, 140, 51]):
                spotted[i][j] = 25
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [255, 179, 51]):
                spotted[i][j] = 30
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [255, 218, 51]):
                spotted[i][j] = 35
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [254, 255, 51]):
                spotted[i][j] = 40
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [215, 255, 90]):
                spotted[i][j] = 45
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [177, 255, 128]):
                spotted[i][j] = 50
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [135, 255, 170]):
                spotted[i][j] = 55
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [96, 255, 20]):
                spotted[i][j] = 60
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [58, 255, 247]):
                spotted[i][j] = 65
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 224, 255]):
                spotted[i][j] = 70
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 185, 255]):
                spotted[i][j] = 75
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 147, 255]):
                spotted[i][j] = 80
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 108, 255]):
                spotted[i][j] = 85
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 70, 255]):
                spotted[i][j] = 90
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 51, 236]):
                spotted[i][j] = 95
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 51, 197]):
                spotted[i][j] = 98
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [51, 51, 159]):
                spotted[i][j] = 100
            elif all(img[int(i * 1.89) + 1211][int(j * 1.81) + 2150] == [0, 0, 0]) and i > 0 and j > 0:
                spotted[i][j] = spotted[i - 1][j - 1]

    txt_file = Dir.files / Dir.dusts
    np.savetxt(txt_file, spotted[:, :], fmt='%i')
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

    main()
