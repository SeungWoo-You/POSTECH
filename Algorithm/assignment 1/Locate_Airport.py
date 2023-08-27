def binary_search(ls: list, target: int, start: int, end: int) -> int:
    if start > end:
        raise ValueError("Cannot find the target")
    mid = (start + end) // 2
    if ls[mid] == target:
        return mid
    elif ls[mid] > target:
        end = mid - 1
    else:
        start = mid + 1
    return binary_search(ls, target, start, end)


def prefix_sum(ls: list) -> list:
    res = [0]
    for i, x in enumerate(ls):
        res.append(res[i] + x)
    return res


def Manhattan(partial: list, idx: int, N: int, x_i: int) -> int:
    left = partial[idx]
    right = partial[-1] - left
    return right - left + (2 * idx - N) * x_i


T = int(input())
for t in range(T):
    N = int(input())
    X, Y = [], []
    points = set()
    for n in range(N):
        x, y = map(int, input().split(' '))
        X.append(x)
        Y.append(y)
        points.add((x, y))
    X.sort(), Y.sort()
    X_partial = prefix_sum(X)
    Y_partial = prefix_sum(Y)
    # 각 부분 합 구하기: n
    res = []
    for x, y in points:
        id_x = binary_search(X, x, 0, N - 1)
        id_y = binary_search(Y, y, 0, N - 1)
        # 이진 탐색으로 리스트에서 값 찾기: log n
        dist = Manhattan(X_partial, id_x, N, x) + \
            Manhattan(Y_partial, id_y, N, y)
        res.append(dist)
    # prefix sum으로 맨해튼 거리 계산: constant
    # res에서 최소 값 찾기: n
    print(min(res))
