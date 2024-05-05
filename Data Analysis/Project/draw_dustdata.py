import os
import cv2
import numpy as np
import logging
import colorlog
from driectory import Directory as Dir


def main():
    txt_file = Dir.files / Dir.dusts
    dust = np.loadtxt(str(txt_file), dtype='uint8')
    logging.info(f"txt_file \033[33m{txt_file}\033[0m load")
    colored_dust = np.zeros((300, 200, 3))

    # image 좌표에 scale 해주자. 원하는 txt 좌표와 img 파일의 크기가 달라서 해당 과정이 필요하다.
    for i in range(300):
        for j in range(200):
            if dust[i][j] == 0:
                colored_dust[i][j] = [255, 255, 255]
            elif dust[i][j] == 2:
                colored_dust[i][j] = [152, 51, 51]
            elif dust[i][j] == 5:
                colored_dust[i][j] = [191, 51, 51]
            elif dust[i][j] == 10:
                colored_dust[i][j] = [230, 51, 51]
            elif dust[i][j] == 15:
                colored_dust[i][j] = [255, 63, 51]
            elif dust[i][j] == 20:
                colored_dust[i][j] = [255, 102, 51]
            elif dust[i][j] == 25:
                colored_dust[i][j] = [255, 140, 51]
            elif dust[i][j] == 30:
                colored_dust[i][j] = [255, 179, 51]
            elif dust[i][j] == 35:
                colored_dust[i][j] = [255, 218, 51]
            elif dust[i][j] == 40:
                colored_dust[i][j] = [254, 255, 51]
            elif dust[i][j] == 45:
                colored_dust[i][j] = [215, 255, 90]
            elif dust[i][j] == 50:
                colored_dust[i][j] = [177, 255, 128]
            elif dust[i][j] == 55:
                colored_dust[i][j] = [135, 255, 170]
            elif dust[i][j] == 60:
                colored_dust[i][j] = [96, 255, 20]
            elif dust[i][j] == 65:
                colored_dust[i][j] = [58, 255, 247]
            elif dust[i][j] == 70:
                colored_dust[i][j] = [51, 224, 255]
            elif dust[i][j] == 75:
                colored_dust[i][j] = [51, 185, 255]
            elif dust[i][j] == 80:
                colored_dust[i][j] = [51, 147, 255]
            elif dust[i][j] == 85:
                colored_dust[i][j] = [51, 108, 255]
            elif dust[i][j] == 90:
                colored_dust[i][j] = [51, 70, 255]
            elif dust[i][j] == 95:
                colored_dust[i][j] = [51, 51, 236]
            elif dust[i][j] == 98:
                colored_dust[i][j] = [51, 51, 197]
            elif dust[i][j] == 100:
                colored_dust[i][j] = [51, 51, 159]

    img_file = Dir.images / Dir.dust_image
    cv2.imwrite(str(img_file), colored_dust)
    logging.info(f"File \033[33m{img_file}\033[0m saved")


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
