from cell import Cell
from zone import Adding, Deleting
import numpy as np
from driectory import Directory as Dir
import logging


class Simulation:
    def __init__(self) -> None:
        self.board: np.ndarray[np.ndarray[Cell]]
        self.read_data()
        self.M: int = self.board.shape[0]  # pixel 수, 3000
        self.N: int = self.board.shape[1]  # pixel 수, 2000
        self.T: int = 100  # 주기

    def run(self, cycle: int) -> None:  # 한 사이클만 처리
        after_board = np.zeros((self.M + 2, self.N + 2),
                               dtype=int)  # 복사용 빈 보드판
        W, H = 3, 3  # 9방향 확산이므로 3x3으로 처리
        for i in range(self.M - W + 1):  # 확산
            for j in range(self.N - H + 1):  # 아이디어로는 맞는데 될듯?
                dusts: np.ndarray[np.ndarray[int]
                                  ] = self.board[i, j].dust.spread()
                after_board[i: i + W, j: j + H] += dusts
        for i in range(self.M):  # 감소 + 추가
            for j in range(self.N):
                cell: Cell = self.board[i, j]
                cell.dust.amount: int = after_board[i + 1, j + 1]  # 먼지 갱신
                if cycle % cell.dust.Mcycle == 0:  # 감소할 주기가 되었으면 감소
                    cell.dust.degenerate()
                if cell.zone.Mcycle != 0 and cycle % cell.zone.Mcycle == 0:  # 추가할 주기가 되었으면 추가
                    if issubclass(type(cell.zone), Adding):
                        cell.dust.amount += cell.zone.generate()
                    if issubclass(type(cell.zone), Deleting):  # 여긴 감소. 근데 실행될 일 X
                        cell.dust.amount -= cell.zone.degenerate()
        return

    def read_data(self) -> None:  # 데이터 파일 읽는 부분
        map_data = Dir.files / Dir.map_data  # 맵 데이터
        board: np.ndarray[np.ndarray[int]] = np.loadtxt(map_data, dtype=int)
        # 로그 찍기. 디버깅용
        logging.info(f"Map \033[33m{map_data} \033[0mloading complete")
        winds_name = Dir.files / Dir.winds  # 바람 데이터
        winds: np.ndarray[np.ndarray[int]] = np.loadtxt(winds_name, dtype=int)
        print('wind_data_complete')
        dust_name = Dir.files / Dir.dusts  # 먼지 데이터
        dusts: np.ndarray[np.ndarray[int]] = np.loadtxt(dust_name, dtype=int)
        vec = np.vectorize(Cell)  # cell 로 이루어진 2차원 배열 생성 함수
        self.board: np.ndarray[np.ndarray[Cell]] = vec(
            amount=dusts, wind_direction=winds, zone=board)  # 2차원 배열 생성
        return

    def afterprocess(self) -> np.ndarray[np.ndarray[int]]:
        board = np.zeros_like(self.board, dtype=int)
        for i in range(self.M):
            for j in range(self.N):
                board[i, j] = self.board[i, j].dust.amount
        return board
