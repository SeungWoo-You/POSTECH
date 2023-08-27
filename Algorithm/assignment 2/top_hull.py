import heapq


def main():
    t = int(input())
    for _ in range(t):
        n = int(input())
        pts = seg_to_pts(n)
        pts.sort(key=lambda p: (p[0], -p[2], p[1][2]))
        res = top_hull(pts)
        res.sort()
        for p in res:
            print(*p)


def seg_to_pts(n: int) -> list:
    res = []
    for _ in range(n):
        seg = list(map(int, input().split()))
        x0 = [seg[0], 'start', seg[2]]
        x1 = [seg[1], 'end', seg[2]]
        res.extend([x0, x1])
    return res


def top_hull(pts: list) -> list:
    maxheap_start = []
    maxheap_end = []
    l = 0
    res = []
    temp = []

    for x, state, y in pts:
        while maxheap_end:
            if maxheap_start[0][1] == maxheap_end[0][1]:
                heapq.heappop(maxheap_start)[1]
                heapq.heappop(maxheap_end)[1]
            else:
                break
        if not maxheap_start:
            res.extend(temp)
            temp.clear()
            l = x
        else:
            y_max = maxheap_start[0][1]
            if y == y_max:
                if not temp or temp[-1][2] != y:
                    temp.append([l, x, y_max])
                    l = x
                else:
                    l = temp[-1][0]
                    temp[-1] = [l, x, y_max]
            elif y > y_max:
                if temp and temp[-1][2] == y_max:
                    l = temp[-1][0]
                    temp[-1] = [l, x, y_max]
                else:
                    temp.append([l, x, y_max])
                l = x
            else:
                pass
        if state == 'start':
            heapq.heappush(maxheap_start, (-y, y))
        elif state == 'end':
            heapq.heappush(maxheap_end, (-y, y))
    res.extend(temp)
    return res


if __name__ == '__main__':
    main()
