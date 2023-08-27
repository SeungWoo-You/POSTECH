import math
from typing import Tuple


def change_polar(x: int, y: int) -> Tuple[float, float]:
    r = math.sqrt(x**2 + y**2)
    theta = math.acos(y / r) if x >= 0 else 2 * math.pi - math.acos(y / r)
    return (r, theta)


def quick_sort(ls: list) -> list:
    if len(ls) <= 1:
        return ls
    pivot = ls[0]
    remains: list = ls[1:]
    left = [one for one in remains if one <= pivot]
    right = [one for one in remains if one > pivot]
    return quick_sort(left) + [pivot] + quick_sort(right)


t = int(input())
for i in range(t):
    N = int(input())
    res_dict = dict()
    for j in range(N):
        x, y = map(int, input().split(' '))
        r, theta = change_polar(x, y)
        if theta in res_dict.keys():
            if r in res_dict[theta].keys():
                res_dict[theta][r].append([x, y])
            else:
                res_dict[theta][r] = [[x, y]]
        else:
            res_dict[theta] = {r: [[x, y]]}
    keys = quick_sort(list(res_dict.keys()))
    print("")
    for k in keys:
        keys2 = quick_sort(list(res_dict[k].keys()))
        for r in keys2:
            for _x, _y in res_dict[k][r]:
                print(_x, _y)
