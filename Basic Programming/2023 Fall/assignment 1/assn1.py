import os
import random


def main():
    board, treadable = mk_stairs()
    pos = [0, 0]
    print_screen(board, pos, init=True)
    N = 0
    while N > 30 or N < 10:
        N = int(input('게임을 위한 계단의 개수를 입력해주세요. <10 ~ 30> >>'))
    board, treadable = mk_stairs(N)
    while N not in pos:
        attack = play_RSP()
        play_MCP(board, pos, treadable, attack)
        print_screen(board, pos)
        if N not in pos:
            enter()
    winner = '플레이어' if pos[0] == N else '컴퓨터'
    for _ in range(13):
        print(f"{'▨':<2}", end='')
    print()
    print(f'  {winner} 최종 승리!!!')
    for _ in range(13):
        print(f"{'▨':<2}", end='')
    print('\n')
    print('게임을 종료합니다...')


def print_screen(board: list[list[str]], pos: list[int], init: bool = False) -> None:
    N = len(board[0]) - 1
    clear_screen()
    status = (
        '======================\n'
        '[묵찌빠 계단 오르기]\n'
        '======================'
    ) if init == True else (
        f'총 계단 수: {N}\n'
        f'PLAYER:   ○ <{pos[0]:2d}>\n'
        f'COMPUTER: ● <{pos[1]:2d}>\n'
    )
    print(status)
    print_stairs(board)


def mk_stairs(N: int = 10) -> list[list[str]]:
    height = max(7, (N + 3) // 2)
    board = [[' '] * (N + 1) for _ in range(height)]
    board[0][0] = '○'
    board[0][-1] = '●'
    treadable = [0] * (N + 1)
    for i in range(1, height):
        for j in range(1, i + 1):
            board[i][j - 1] = board[i][-j] = '▨'
        treadable[i] = i if treadable[i] == 0 else treadable[i]
        treadable[-i - 1] = i if treadable[-i - 1] == 0 else treadable[-i - 1]
    return board, treadable


def print_stairs(board: list[list[str]]) -> None:
    for row in board:
        for x in row:
            print(f'{x:<2}', end='')
        print()
    print()


def play_RSP() -> int:
    clear_screen()
    winner = 0
    while winner == 0:
        print('[공격권 결정 가위바위보]')
        user = ''
        com = ''
        while user not in HAND:
            user = input('가위, 바위, 보 중 하나 선택:')
        com = computer_choice()
        print('[컴퓨터 선택]')
        print_hand(com)
        print('[플레이어 선택]')
        print_hand(user)
        winner = order(user, com)
        if winner == 0:
            print('[결과] 무승부입니다.')
        elif winner == 1:
            print('[결과] 컴퓨터 공격, 플레이어 수비입니다.')
        elif winner == -1:
            print('[결과] 플레이어 공격, 컴퓨터 수비입니다.')
        print()
        enter()
        clear_screen()
    return winner


def play_MCP(board: list[list[str]], pos: list[int], treadable: list[int], start: int) -> None:
    clear_screen()
    N = len(board[0]) - 1
    turn = 1
    attacker = '컴퓨터' if start == 1 else '플레이어'
    defender = '컴퓨터' if attacker == '플레이어' else '플레이어'
    winner = start
    while winner != 0:
        print('[묵찌빠]')
        print(f'승리 시 이동 칸 수: {turn}')
        print(f'{attacker} 공격, {defender} 수비입니다.')
        user = ''
        com = ''
        while user not in HAND:
            user = input('가위, 바위, 보 중 하나 선택:')
        com = computer_choice()
        print('[컴퓨터 선택]')
        print_hand(com)
        print('[플레이어 선택]')
        print_hand(user)
        winner = order(user, com)
        if winner == 0:
            print('[결과] 묵찌빠 종료')
            print(f'{attacker} 승, {turn}칸 이동합니다.')
            if attacker == '플레이어':
                x, y = treadable[pos[0]], pos[0]
                board[x][y] = 'o' if board[x][y] == '◑' else ' '
                pos[0] = min(N, pos[0] + turn)
                x, y = treadable[pos[0]], pos[0]
                board[x][y] = '○' if board[x][y] != '●' else '◑'
            else:
                x, y = treadable[N - pos[1]], N - pos[1]
                board[x][y] = '●' if board[x][y] == '◑' else ' '
                pos[1] = min(N, pos[1] + turn)
                x, y = treadable[N - pos[1]], N - pos[1]
                board[x][y] = '●' if board[x][y] != '○' else '◑'
        else:
            if winner == 1:
                attacker = '컴퓨터'
                defender = '플레이어'
            else:
                attacker = '플레이어'
                defender = '컴퓨터'
            print(f'{attacker} 공격, {defender} 수비입니다.')
        print()
        enter()
        clear_screen()
        turn += 1
    return


def order(h1: str, h2: str) -> int:
    table = ['바위', '가위', '보']
    if (table.index(h2) + 1) % 3 == table.index(h1):
        return 1
    if h1 == h2:
        return 0
    return -1


def computer_choice() -> str:
    return random.choice(HAND)


def print_hand(hand: str) -> None:
    if hand == '바위':
        print_rock()
    elif hand == '가위':
        print_scissors()
    elif hand == '보':
        print_paper()


def print_rock() -> None:
    for x in ROCK:
        print(f'{x:<2}' if x == '▩' else x, end='')
    print()


def print_scissors() -> None:
    for x in SCISSORS:
        print(f'{x:<2}' if x == '▩' else x, end='')
    print()


def print_paper() -> None:
    for x in PAPER:
        print(f'{x:<2}' if x == '▩' else x, end='')
    print()


def clear_screen() -> None:
    os.system('cls')


def enter() -> None:
    return input('계속하려면 엔터를 눌러주세요...')


if __name__ == '__main__':
    ROCK = (
        '┌───────────────────────────────────────┐\n'
        '│                                       │\n'
        '│         ▩▩▩▩▩                    │\n'
        '│       ▩▩▩▩▩▩▩▩▩              │\n'
        '│     ▩▩▩▩▩▩▩▩▩▩▩            │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩            │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩▩          │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩▩          │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩▩          │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩            │\n'
        '│     ▩▩▩▩▩▩▩▩▩▩              │\n'
        '│       ▩▩▩▩▩▩▩                  │\n'
        '│                                       │\n'
        '└───────────────────────────────────────┘'
    )
    SCISSORS = (
        '┌───────────────────────────────────────┐\n'
        '│                             ▩▩      │\n'
        '│             ▩▩        ▩▩▩▩▩    │\n'
        '│         ▩▩▩▩▩    ▩▩▩▩▩▩▩  │\n'
        '│     ▩▩▩▩▩▩▩▩▩▩▩▩▩▩      │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩            │\n'
        '│   ▩▩▩▩▩▩▩▩▩                  │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩              │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩▩          │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩      │\n'
        '│     ▩▩▩▩▩▩▩  ▩▩▩▩▩▩▩    │\n'
        '│       ▩▩▩▩▩▩      ▩▩▩▩▩    │\n'
        '│           ▩▩▩            ▩▩      │\n'
        '└───────────────────────────────────────┘'
    )
    PAPER = (
        '┌───────────────────────────────────────┐\n'
        '│                                       │\n'
        '│         ▩▩▩▩▩                    │\n'
        '│       ▩▩▩                          │\n'
        '│     ▩▩▩▩▩▩▩▩▩▩▩▩▩▩      │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩                │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩    │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩                │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩▩▩▩▩▩      │\n'
        '│   ▩▩▩▩▩▩▩▩▩▩                │\n'
        '│     ▩▩▩▩▩▩▩▩▩▩▩▩          │\n'
        '│       ▩▩▩▩▩                      │\n'
        '│                                       │\n'
        '└───────────────────────────────────────┘'
    )
    HAND = ['가위', '바위', '보']
    main()
