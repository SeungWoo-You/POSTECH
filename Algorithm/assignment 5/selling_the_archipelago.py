import sys
import heapq


class Archipelago:
    def __init__(self, I: int, P: int, B: int) -> None:
        self.N = I
        self.P = P
        self.B = B
        self.portals = set()
        self.islands = []
        self.heads = list(range(I))
        self.keep = dict()
        self.mk_graph()

    def mk_graph(self) -> None:
        self.portals = set(map(int, input().split()))
        for _ in range(self.B):
            I1, I2, C = map(int, input().split())
            self.islands.append((C, I1, I2))
        self.islands.sort()
        self.keep = {vertex: {} for vertex in self.portals}

    def find_head(self, v: int) -> int:
        heads = self.heads
        if heads[v] != v:
            heads[v] = self.find_head(heads[v])
        return heads[v]

    def union_head(self, gp1: int, gp2: int) -> bool:
        gp = {gp1, gp2}
        if gp & self.portals == {gp1}:
            self.heads[gp2] = self.heads[gp1]
        elif gp & self.portals == {gp1, gp2}:
            return False
        self.heads[gp1] = self.heads[gp2]
        return True

    def connect_islands(self) -> None:
        for w, u, v in self.islands:
            gp1, gp2 = self.find_head(u), self.find_head(v)
            if gp1 != gp2:
                if self.union_head(gp1, gp2) == False:
                    if gp2 not in self.keep[gp1]:
                        self.keep[gp1][gp2] = 0
                    self.keep[gp1][gp2] = self.keep[gp2][gp1] = max(
                        self.keep[gp1][gp2], w)
        return

    def find_max_worth(self) -> int:
        visited = {v: False for v in self.portals}
        start = list(self.portals)[0]
        heap = [(0, start)]
        res = 0
        while heap:
            weight, node = heapq.heappop(heap)
            if visited[node] == True:
                continue
            res -= weight
            visited[node] = True
            for v in self.keep[node]:
                heapq.heappush(heap, (-self.keep[node][v], v))
        return res


def main():
    t = int(input())
    for _ in range(t):
        I, P, B = map(int, input().split())
        graph = Archipelago(I, P, B)
        graph.connect_islands()
        print(graph.find_max_worth())


if __name__ == '__main__':
    sys.setrecursionlimit(10**5)
    main()
