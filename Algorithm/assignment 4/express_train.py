from collections import defaultdict
import math
import heapq


def main():
    T = int(input())
    for _ in range(T):
        N, M = map(int, input().split())
        s, t = map(int, input().split())
        lines = defaultdict(list)
        for _ in range(M):
            U, V, L, E = map(int, input().split())
            lines[U].append((V, L, E))
        res = find_dist(lines, N, s, t)
        print(res) if res != math.inf else print(-1)


def find_dist(lines: defaultdict, N: int, s: int, t: int) -> int:
    costs = [math.inf] * N
    costs[0] = 0
    queue = []
    heapq.heappush(queue, (costs[0], 0))
    while queue:
        dist, U = heapq.heappop(queue)
        for V, L, E in lines[U]:
            temp = dist
            if E == 1:
                if temp < s:
                    temp = s
                if temp + L > t:
                    continue
            if temp + L < costs[V]:
                costs[V] = temp + L
                heapq.heappush(queue, (costs[V], V))
    return costs[-1]


if __name__ == '__main__':
    main()
