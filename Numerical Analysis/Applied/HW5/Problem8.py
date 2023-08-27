import os
import numpy as np

from methods import nonlinfin_difference

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

    def given_f(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        Y = (1 + yp**2)**(-3 / 2)
        Q = S / (E * I)
        R = q * x / (2 * E * I) * (x - l)
        return (Q * y + R) / Y

    def given_fy(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        Y = (1 + yp**2)**(-3 / 2)
        Q = S / (E * I)
        return Q / Y

    def given_fyp(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        Q = S / (E * I)
        R = q * x / (2 * E * I) * (x - l)
        return (Q * y + R) * 3 * yp * (1 + yp**2)**(1 / 2)

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
    res = nonlinfin_difference(bvp, 10, use_TOL=False)
    print(f"Linear Finite-Difference with h = {h}")
    print(res)
    print("")


if __name__ == "__main__":
    main()
