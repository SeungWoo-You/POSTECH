from dataclasses import dataclass
from zone import Zone
import importlib
import numpy as np


@dataclass
class State:
    rate: float
    Mcycle: int


class Cell:
    def __init__(self, amount: int, wind_direction: int, zone: int) -> None:
        self.wind: Wind = Wind(0.1, 1, wind_direction)
        self.dust: Dust = Dust(amount * 1000, self.calc_probs())
        module = importlib.import_module('zone')
        self.zone: Zone = getattr(module, self.ID[zone])()

    @property
    def ID(self) -> dict[int, str]:
        return {
            0: 'Sea',
            1: 'Land',
            2: 'Factory'
        }

    @property
    def WAY(self) -> dict[int, tuple[int, int]]:
        return {
            0: (-1, -1),  # 좌상
            1: (-1, 0),  # 상
            2: (-1, 1),  # 우상
            3: (0, -1),  # 좌
            4: (0, 0),  # 중앙
            5: (0, 1),  # 우
            6: (1, -1),  # 좌하
            7: (1, 0),  # 하
            8: (1, 1)  # 우하
        }

    def calc_probs1(self) -> np.ndarray[float]:
        DIRECTION = {
            0: [1, 3],
            1: [0, 2],
            2: [1, 5],
            3: [0, 6],
            5: [2, 8],
            6: [3, 7],
            7: [6, 8],
            8: [7, 5]
        }
        probs = np.array([0] * 9, dtype=float)
        for i in DIRECTION[self.wind.direction]:
            probs[i] = 0.2
        probs[4] = probs[self.wind.direction] = 0.3
        return probs

    def calc_probs(self) -> np.ndarray[float]:  # 9방향 확산
        # 균등하게 분할 후 해당 방향으로만 추가 가중치 부여
        decrease_weight = self.wind.rate / 9
        probs = np.array([1 / 9] * 9, dtype=float)
        probs -= decrease_weight
        probs[self.wind.direction] += self.wind.rate
        np.place(probs, probs < 0, 0)  # 확률 0 미만은 보정
        np.place(probs, probs > 1, 1)  # 확률 0 초과는 보정
        return probs


class Dust(State):
    def __init__(self, amount: int, probs: np.ndarray[float]) -> None:
        super().__init__(0.05, 4)
        self.amount: int = amount
        self.probs: np.ndarray[float] = probs

    def spread(self) -> np.ndarray[np.ndarray[int]]:
        # 분배 확률에 따라 먼지 양 9칸에 분배, 이후 3x3 배열로 변경
        return np.multiply(self.probs, self.amount, dtype=float).astype(int).reshape((3, 3))

    def degenerate(self) -> None:  # rate 만큼 먼지 감소
        self.amount = max(0, int((1 - self.rate) * self.amount))
        return


@dataclass
class Wind(State):  # rate: 확산 가중치, Mcycle: 더미데이터
    # 상속 안쓰는것도 고민 중..
    direction: int
