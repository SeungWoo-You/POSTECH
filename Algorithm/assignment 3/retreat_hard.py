import sys
import math
from collections import defaultdict


class Graph:
    def __init__(self, N: int, M: int) -> None:
        self.bridges = defaultdict(int)
        self.graph = defaultdict(list)
        for _ in range(M):
            land1, land2, brig = map(int, input().split())
            self.bridges[tuple(sorted([land1, land2]))] = brig
            self.graph[land1].append(land2)
            self.graph[land2].append(land1)
        self.heads = [0 for _ in range(N)]
        self.header = 0
        self.breakables = set()
        self.touches = [0 for _ in range(N)]

    def DFS(self, v: int, prev: int):
        self.header += 1
        self.heads[v] = self.header
        now_header = self.header
        count = 0
        for x in self.graph[v]:
            if x == prev:
                count += 1
                self.touches[v] = count
                continue
            elif self.heads[x] > 0:
                now_header = min(now_header, self.heads[x])
                continue
            else:
                tracked_header = self.DFS(x, v)
                now_header = min(now_header, tracked_header)
                if tracked_header > self.heads[v] and self.touches[x] < 2:
                    self.breakables.add((x, v))
        return now_header


def main():
    T = int(input())
    for _ in range(T):
        N, M = map(int, input().split())
        gph = Graph(N, M)
        gph.DFS(0, 0)
        print(blow_bridge(gph))


def blow_bridge(graph: Graph) -> int:
    cost = math.inf
    for x, y in graph.breakables:
        i = tuple(sorted([x, y]))
        here = graph.bridges[i]
        if cost > here:
            cost = here
    if cost == math.inf:
        cost = -1
    return cost


if __name__ == '__main__':
    sys.setrecursionlimit(10**5)
    main()
