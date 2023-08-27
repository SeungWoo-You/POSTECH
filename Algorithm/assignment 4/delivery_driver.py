from collections import defaultdict, deque


def topo_sort(graph: dict, deg: dict) -> list:
    queue = deque([])
    for v in graph:
        if deg[v] == 0:
            queue.append(v)
    ordered = []

    while queue:
        node = queue.popleft()
        ordered.append(node)
        for v in graph[node]:
            deg[v] -= 1
            if deg[v] == 0:
                queue.append(v)
    return ordered


def count_paths(graph: defaultdict, order: list, src: int, dst: int, must: set) -> int:
    temp = defaultdict(int)
    temp[src] = 1
    count = 1
    for node in order:
        if node in must:
            count = (count * temp[node]) % 20230419
            if count == 0:
                return count
            temp = defaultdict(int)
            temp[node] = 1
        if node == dst:
            count = (count * temp[node]) % 20230419
            return count
        for v in graph[node]:
            temp[v] += temp[node]
    return count


def main():
    T = int(input())
    for _ in range(T):
        N, M = map(int, input().split())
        sects = {v: set() for v in range(N)}
        deg = {v: 0 for v in range(N)}
        for _ in range(M):
            u, v = map(int, input().split())
            sects[u].add(v)
            deg[v] += 1
        S, E, K = map(int, input().split())
        must = set(map(int, input().split()))
        topo = topo_sort(sects, deg)
        print(count_paths(sects, topo, S, E, must))


if __name__ == '__main__':
    main()
