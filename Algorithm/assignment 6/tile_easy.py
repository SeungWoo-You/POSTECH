import sys


def main():
    T = int(input())
    counts = [1, 2, 7]
    cumuls = [0, 0, 0]
    for _ in range(T):
        N = int(input())
        if len(counts) <= N:
            while len(counts) <= N:
                cumuls.append((cumuls[-1] + counts[-3]) % MOD)
                counts.append(
                    (2 * counts[-1] + 3 * counts[-2] + 2 * cumuls[-1]) % MOD)
        print(counts[N])


if __name__ == "__main__":
    input = sys.stdin.readline
    MOD = 10**9
    main()
