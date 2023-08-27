def main():
    t = int(input())
    for _ in range(t):
        n, w = map(int, input().split())
        hulls = []
        for _ in range(n):
            l, r, y = map(int, input().split())
            hulls.append((l, r, y))
        print(find_max_area(hulls, n, w))


def find_max_area(hulls: list, N: int, W: int) -> int:
    area = 0
    heights = []
    i = 0

    while i < N:
        if not heights or heights[-1][2] <= hulls[i][2]:
            heights.append(hulls[i])
            i += 1
        else:
            top = heights.pop()[2]
            side = hulls[i][0] - heights[-1][1] if heights else hulls[i][0]
            temp_area = top * side
            area = max(area, temp_area)

    while heights:
        top = heights.pop()[2]
        side = W - heights[-1][1] if heights else W
        temp_area = top * side
        area = max(area, temp_area)
    return area


if __name__ == '__main__':
    main()
