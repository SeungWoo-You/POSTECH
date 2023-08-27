import os
import numpy as np

from methods import linear_shooting


class BVP:
    def __init__(self, h: float, boundary_x: tuple[float, float], boundary_y: tuple[float, float]) -> None:
        self.h = h
        self.boundary_x = boundary_x
        self.boundary_y = boundary_y
        self.N = int((boundary_x[1] - boundary_x[0]) / h)

    def given_p(self, x: float | np.ndarray) -> float | np.ndarray:
        return -2 / x

    def given_q(self, x: float | np.ndarray) -> float | np.ndarray:
        return 0

    def given_r(self, x: float | np.ndarray) -> float | np.ndarray:
        return 0

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        return self.boundary_y[0] * self.boundary_x[0] / x * ((self.boundary_x[1] - x) / (self.boundary_x[1] - self.boundary_x[0]))


def main():
    os.system("cls")
    h = 0.1
    bvp = BVP(h, (2, 4), (110, 0))
    res = linear_shooting(bvp)
    print(f"Linear Shooting with h = {h}")
    print(res)
    print("")


if __name__ == "__main__":
    main()
