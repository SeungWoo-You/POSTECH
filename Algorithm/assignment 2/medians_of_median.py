import sys


def main():
    t = int(input())
    for _ in range(t):
        N = int(input())
        ls = list(map(int, input().split()))
        print(quick_select(ls, (N + 1) // 2))


def mom(ls: list) -> int:
    n = 5
    div_5 = [ls[i * n:(i + 1) * n] for i in range((len(ls) + n - 1) // n)]
    medians = [find_median(s) for s in div_5]
    if len(medians) <= 2:
        return min(medians)
    return mom(medians)


def find_median(ls: list) -> int:
    if len(ls) <= 2:
        return min(ls)
    else:
        ls.remove(max(ls))
        ls.remove(min(ls))
        return find_median(ls)


def quick_select(ls: list, k: int) -> int:
    smaller, bigger, same = [], [], []
    pivot = mom(ls)
    if len(ls) == 1:
        return ls[0]
    for x in ls:
        if x < pivot:
            smaller.append(x)
        elif x > pivot:
            bigger.append(x)
        else:
            same.append(x)
    size_s, size_m = len(smaller), len(same)
    if size_s >= k:
        return quick_select(smaller, k)
    elif size_s + size_m < k:
        return quick_select(bigger, k - (size_s + size_m))
    else:
        return pivot


if __name__ == '__main__':
    sys.setrecursionlimit(10**7)
    main()
