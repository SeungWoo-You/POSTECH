import sys


def main():
    T = int(input())
    for _ in range(T):
        N = int(input())
        if N < 2:
            res = [1, 2, 7]
            print(res[N])
            continue
        mat = calc_mat_power([[3, 1, -1], [1, 0, 0], [0, 1, 0]], N - 2)
        A1 = (mat[0][0] * 7) % MOD
        A2 = (mat[0][1] * 2) % MOD
        A3 = (mat[0][2] * 1) % MOD
        print((A1 + A2 + A3) % MOD)


def calc_mat_power(M: list, n: int) -> list:
    if n == 0:
        return [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
    elif n == 1:
        return M
    elif n == 2:
        return product_mat(M, M)
    else:
        if n % 2 == 1:
            temp = calc_mat_power(M, (n - 1) // 2)
            temp = product_mat(temp, temp)
            return product_mat(temp, M)
        else:
            temp = calc_mat_power(M, n // 2)
            return product_mat(temp, temp)


def product_mat(M1: list, M2: list) -> list:
    a11 = (M1[0][0] * M2[0][0] + M1[0][1] *
           M2[1][0] + M1[0][2] * M2[2][0]) % MOD
    a12 = (M1[0][0] * M2[0][1] + M1[0][1] *
           M2[1][1] + M1[0][2] * M2[2][1]) % MOD
    a13 = (M1[0][0] * M2[0][2] + M1[0][1] *
           M2[1][2] + M1[0][2] * M2[2][2]) % MOD
    a21 = (M1[1][0] * M2[0][0] + M1[1][1] *
           M2[1][0] + M1[1][2] * M2[2][0]) % MOD
    a22 = (M1[1][0] * M2[0][1] + M1[1][1] *
           M2[1][1] + M1[1][2] * M2[2][1]) % MOD
    a23 = (M1[1][0] * M2[0][2] + M1[1][1] *
           M2[1][2] + M1[1][2] * M2[2][2]) % MOD
    a31 = (M1[2][0] * M2[0][0] + M1[2][1] *
           M2[1][0] + M1[2][2] * M2[2][0]) % MOD
    a32 = (M1[2][0] * M2[0][1] + M1[2][1] *
           M2[1][1] + M1[2][2] * M2[2][1]) % MOD
    a33 = (M1[2][0] * M2[0][2] + M1[2][1] *
           M2[1][2] + M1[2][2] * M2[2][2]) % MOD
    return [[a11, a12, a13], [a21, a22, a23], [a31, a32, a33]]


if __name__ == "__main__":
    input = sys.stdin.readline
    MOD = 1234567890
    main()
