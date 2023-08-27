import heapq


def main():
    t = int(input())
    for _ in range(t):
        N, M = map(int, input().split())
        road = [list(map(int, input().split())) for _ in range(N)]
        print(find_speed(road, N, M))


def find_speed(road: list, N: int, M: int) -> int:
    visited = [[False] * M for _ in range(N)]
    max_heap = [(-road[0][0], 0, 0)]
    heapq.heapify(max_heap)
    speed = -max_heap[0][0]
    i, j = 0, 0
    visited[0][0] = True

    while visited[N - 1][M - 1] != True:
        stand_speed, i, j = heapq.heappop(max_heap)
        stand_speed *= -1
        if stand_speed < speed:
            speed = stand_speed

        top = (i - 1, j) if i > 0 else None
        bot = (i + 1, j) if i < N - 1 else None
        left = (i, j - 1) if j > 0 else None
        right = (i, j + 1) if j < M - 1 else None
        direction = []
        for dr in [top, bot, left, right]:
            if dr != None:
                direction.append(dr)
        for dr in direction:
            x, y = dr
            if visited[x][y] == True:
                continue
            heapq.heappush(max_heap, (-road[x][y], x, y))
            visited[x][y] = True

    if road[-1][-1] < speed:
        speed = road[-1][-1]
    return speed


if __name__ == '__main__':
    main()
