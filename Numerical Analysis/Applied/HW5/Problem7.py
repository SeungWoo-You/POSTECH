import os
import numpy as np

from methods import nonlinfin_difference


class BVP:
    def __init__(self, h: float, boundary_x: tuple[float, float], boundary_y: tuple[float, float]) -> None:
        self.h = h
        self.boundary_x = boundary_x
        self.boundary_y = boundary_y
        self.N = int((boundary_x[1] - boundary_x[0]) / h)
        self.TOL = 10**(-4)

    def given_f(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return yp + 2 * (y - np.log(x))**3 - x**(-1)

    def given_fy(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return 6 * (y - np.log(x))**2

    def given_fyp(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return 1

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        return x**(-1) + np.log(x)


def main():
    os.system("cls")
    h = 0.1
    bvp = BVP(h, (2, 3), (1 / 2 + np.log(2), 1 / 3 + np.log(3)))
    res = nonlinfin_difference(bvp, use_TOL=True)
    print(f"Nonlinear Finite-Difference with h = {h}")
    print(res)
    print("")


if __name__ == "__main__":
    main()
