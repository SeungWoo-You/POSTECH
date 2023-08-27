import os
import numpy as np

from methods import linfin_difference

l = 120
q = 100 / 12
E = 3.0 * 10**7
S = 1000
I = 625


class BVP:
    def __init__(self, h: float, boundary_x: tuple[float, float], boundary_y: tuple[float, float]) -> None:
        self.h = h
        self.boundary_x = boundary_x
        self.boundary_y = boundary_y
        self.N = int((boundary_x[1] - boundary_x[0]) / h)

    def given_p(self, x: float | np.ndarray) -> float | np.ndarray:
        return 0

    def given_q(self, x: float | np.ndarray) -> float | np.ndarray:
        return S / (E * I)

    def given_r(self, x: float | np.ndarray) -> float | np.ndarray:
        return q * x / (2 * E * I) * (x - l)

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        alpha = S / (E * I)
        beta = q / (2 * E * I)
        a = np.sqrt(alpha)  # 2.3094010 * 10**(-4)
        b = -beta / alpha  # -4.1666666 * 10**(-3)
        c = -2 * beta / alpha**2  # -1.5625 * 10**5
        c1 = -c * 1 / (np.exp(a * l) + 1)  # 7.7042537 * 10**4
        c2 = -c * np.exp(a * l) / (np.exp(a * l) + 1)  # 7.9207462 * 10**4
        return c1 * np.exp(np.sqrt(alpha) * x) + c2 * np.exp(-np.sqrt(alpha) * x) + b * (x - l) * x + c


def main():
    os.system("cls")
    h = 6
    bvp = BVP(h, (0, l), (0, 0))
    res = linfin_difference(bvp)
    print(f"Linear Finite-Difference with h = {h}")
    print(res)
    print("")


if __name__ == "__main__":
    main()
