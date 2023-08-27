import sys


def main():
    t = int(input())
    for _ in range(t):
        N, M, d = map(int, input().split())
        sum_ls = [[0] * (M + 1) for _ in range(N + 1)]
        I = []

        for i in range(1, N + 1):
            I.append(list(map(int, input().split())))
            for j in range(1, M + 1):
                if j == 1:
                    sum_ls[i][j] = I[i - 1][j - 1]
                else:
                    sum_ls[i][j] = I[i - 1][j - 1] + sum_ls[i][j - 1]

        for i in range(1, N + 1):
            for j in range(1, M + 1):
                sum_ls[i][j] += sum_ls[i - 1][j]

        res = calc_convol(sum_ls, N, M, d)
        for row in res:
            print(*row)


def calc_convol(sum_ls: list, N: int, M: int, d: int) -> list:
    n, m = N - d + 1, M - d + 1
    res = [[0] * m for _ in range(n)]
    for i in range(n):
        for j in range(m):
            x1, y1 = i, j
            x2, y2 = i + d, j + d
            res[i][j] = sum_ls[x2][y2] - sum_ls[x2][y1] - \
                sum_ls[x1][y2] + sum_ls[x1][y1]
    return res


if __name__ == "__main__":
    input = sys.stdin.readline
    main()
