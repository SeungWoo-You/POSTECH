import os
import numpy as np

from methods import nonlin_shooting


class BVP:
    def __init__(self, h: float, boundary_x: tuple[float, float], boundary_y: tuple[float, float]) -> None:
        self.h = h
        self.boundary_x = boundary_x
        self.boundary_y = boundary_y
        self.N = int((boundary_x[1] - boundary_x[0]) / h)
        self.TOL = 10**(-5)

    def given_f(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return -yp ** 2 - y + np.log(x)

    def given_fy(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return -1

    def given_fyp(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return -2 * yp

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        return np.log(x)


def main():
    os.system("cls")
    h = 0.5
    bvp = BVP(h, (1, 2), (0, np.log(2)))
    res = nonlin_shooting(bvp, 10, use_TOL=False)
    print(f"Nonlinear Shooting with h = {h}")
    print(res)
    print("")


if __name__ == "__main__":
    main()
