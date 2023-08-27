import sys
import math
from collections import defaultdict


class Graph:
    def __init__(self, N: int, M: int) -> None:
        self.bridges = defaultdict(int)
        self.graph = defaultdict(set)
        for _ in range(M):
            land1, land2, brig = map(int, input().split())
            self.bridges[tuple(sorted([land1, land2]))] = brig
            self.graph[land1].add(land2)
            self.graph[land2].add(land1)
        self.heads = [0 for _ in range(N)]
        self.header = 0
        self.breakables = set()

    def DFS(self, v: int, prev: int) -> int:
        self.header += 1
        self.heads[v] = self.header
        now_header = self.header
        for x in self.graph[v]:
            if x == prev:
                continue
            elif self.heads[x] > 0:
                now_header = min(now_header, self.heads[x])
                continue
            else:
                tracked_header = self.DFS(x, v)
                now_header = min(now_header, tracked_header)
                if tracked_header > self.heads[v]:
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
