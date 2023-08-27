from itertools import combinations


def main():
    t = int(input())
    for _ in range(t):
        N = int(input())
        pts = [list(map(int, input().split())) for _ in range(N)]
        pts.sort()
        print(closest_dist(pts))


def closest_dist(pts: list) -> int:
    N = len(pts)
    if N <= 3:
        res = [Manhattan(p1, p2) for p1, p2 in combinations(pts, 2)]
        return min(res)

    n = N // 2
    l_pts, r_pts = pts[:n], pts[n:]
    left = closest_dist(l_pts)
    right = closest_dist(r_pts)
    dist = min(left, right)

    center_x = pts[n][0]
    mid = [p for p in pts if abs(p[0] - center_x) <= dist]
    mid.sort(key=lambda p: p[1])
    M = len(mid)

    for i in range(M):
        for j in range(i + 1, M):
            if abs(mid[i][1] - mid[j][1]) >= dist:
                break
            else:
                dist = min(dist, Manhattan(mid[i], mid[j]))
    return dist


def Manhattan(p1: list, p2: list) -> int:
    x_dist = abs(p1[0] - p2[0])
    y_dist = abs(p1[1] - p2[1])
    return x_dist + y_dist


if __name__ == '__main__':
    main()
