import os
import numpy as np
import pandas as pd

from methods import linfin_difference, extrapol


class BVP:
    def __init__(self, h: float, boundary_x: tuple[float, float], boundary_y: tuple[float, float]) -> None:
        self.h = h
        self.boundary_x = boundary_x
        self.boundary_y = boundary_y
        self.N = int((boundary_x[1] - boundary_x[0]) / h)

    def given_p(self, x: float | np.ndarray) -> float | np.ndarray:
        return 1

    def given_q(self, x: float | np.ndarray) -> float | np.ndarray:
        return 2

    def given_r(self, x: float | np.ndarray) -> float | np.ndarray:
        return np.cos(x)

    def exact_sol(self, x: float | np.ndarray) -> float | np.ndarray:
        return - 1 / 10 * (np.sin(x) + 3 * np.cos(x))


def problem_ab(h: float) -> pd.DataFrame:
    bvp = BVP(h, (0, np.pi / 2), (-0.3, -0.1))
    res = linfin_difference(bvp)
    print(f"Linear Finite-Difference with h = {h}")
    print(res)
    print("")
    return res


def problem_c(w1: pd.DataFrame, w2: pd.DataFrame) -> None:
    w2 = w2.iloc[::2, :].reset_index()
    res_w = extrapol(w1['nume_sol'], w2['nume_sol'], 1)
    w1['nume_sol'] = res_w
    print("Extrapolation result:")
    print(w1)
    return


def main():
    os.system("cls")
    hs = [np.pi / 4, np.pi / 8]

    w1 = problem_ab(hs[0])
    w2 = problem_ab(hs[1])
    problem_c(w1, w2)


if __name__ == "__main__":
    main()
