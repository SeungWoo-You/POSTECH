import numpy as np
from dataclasses import dataclass


@dataclass
class State:
    rate: float
    Mcycle: int


class Zone(State):
    def __init__(self, is_throughable: bool, **kwargs) -> None:
        self.is_throughable: bool = is_throughable  # 더미변수


class Adding:
    def __init__(self, emmision: int, **kwargs) -> None:
        self.emmision = emmision  # 방출하는 양의 평균

    def generate(self) -> int:  # 정규분포로 방출량 결정
        return np.random.normal(self.emmision, np.sqrt(self.emmision))


class Deleting:
    def __init__(self, reduction: int) -> None:
        self.reduction = reduction

    def degenerate(self) -> int:  # 더미 코드
        pass


class Factory(Adding):
    def __init__(self) -> None:
        super().__init__(emmision=2500)
        self.Mcycle = 2
        # super(Zone, self).__init__(0, 3)  # rate: 더미변수


class Nature(Zone, Deleting):  # 더미 코드
    def __init__(self) -> None:
        pass


class Sea(Zone):
    def __init__(self) -> None:
        super().__init__(is_throughable=True)
        super(Zone, self).__init__(0, 0)  # 주기에 따라 추가, 감소 없으면 Mcycle = 0


class Land(Zone):
    def __init__(self) -> None:
        super().__init__(is_throughable=True)
        super(Zone, self).__init__(0, 0)
