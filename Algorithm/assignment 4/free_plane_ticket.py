from collections import defaultdict
import math
import heapq


def main():
    T = int(input())
    for _ in range(T):
        V, E = map(int, input().split())
        ports = defaultdict(dict)
        ports_reversed = defaultdict(dict)
        checked = set()
        for _ in range(E):
            u, v, p = map(int, input().split())
            if (u, v) in checked:
                if ports[u][v] < p:
                    continue
            ports[u][v] = p
            ports_reversed[v][u] = p
            checked.add((u, v))
        src_to_all = costs_for_all(ports, V, 0)
        all_to_dst = costs_for_all(ports_reversed, V, V - 1)
        print(find_cost(checked, src_to_all, all_to_dst))


def costs_for_all(graph: defaultdict, N: int, src: int) -> list:
    costs = [math.inf] * N
    costs[src] = 0
    queue = []
    heapq.heappush(queue, (costs[src], src))
    while queue:
        dist, u = heapq.heappop(queue)
        for v, p in graph[u].items():
            item = (dist + p, v)
            if item[0] < costs[v]:
                costs[v] = item[0]
                heapq.heappush(queue, item)
    return costs


def find_cost(edges: set, s_to_v: list, w_to_d: list) -> int:
    cost = math.inf
    for v, w in edges:
        cost = min(s_to_v[v] + w_to_d[w], cost)
    return cost if cost != math.inf else -1


if __name__ == '__main__':
    main()
