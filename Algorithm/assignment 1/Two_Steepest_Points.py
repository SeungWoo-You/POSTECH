T = int(input())
for t in range(T):
    N = int(input())
    points = []
    for n in range(N):
        x, y = map(int, input().split(' '))
        points.append([x, y])
    steepest = 0
    res_points = []
    points.sort()
    for i, point in enumerate(points):
        p1 = points[i]
        try:
            p2 = points[i + 1]
        except:
            break
        steep = abs((p2[1] - p1[1]) / (p2[0] - p1[0]))
        if steep > steepest:
            res_points = [*p1, *p2]
            steepest = steep
    print(*res_points)
