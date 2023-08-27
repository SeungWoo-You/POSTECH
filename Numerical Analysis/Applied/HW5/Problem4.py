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
        return 0.5 * (y**2 - 1) * yp - y

    def given_fy(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return 2 * 0.5 * y * yp - 1

    def given_fyp(self, x: float | np.ndarray, y: float | np.ndarray, yp: float | np.ndarray) -> float | np.ndarray:
        return 0.5 * (y**2 - 1)

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        return 0


def main():
    os.system("cls")
    h = 0.2
    bvp = BVP(h, (0, 2), (0, 1))
    res = nonlin_shooting(bvp, 10, use_TOL=False)
    res.drop(index=[0, 10], columns=['exact_sol', 'error'], inplace=True)
    print(f"Nonlinear Shooting with h = {h}")
    print(res)
    print("")


if __name__ == "__main__":
    main()
