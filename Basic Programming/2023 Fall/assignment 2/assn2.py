import os
import random
from pathlib import Path
from collections import Counter
import time


def main():
    while True:
        sel = init_game()
        if sel == 1:
            play()
        if sel == 2:
            score_list = load_file2list()
            turn = 1
            for c, i in CATEGORY.items():
                if c not in ['sub', 'bonus', 'T']:
                    turn = turn + 1 if score_list[0][i] != 'x' else turn
            play(score_list, turn)
        if sel == 3:
            print('Program ended. Bye!')
            return


def init_game() -> int:
    os.system('cls')
    init: tuple[str, ...] = (
        '[Yacht Dice]',
        '------------------------------------',
        '1. New Game   2. Load Game   3. Exit',
        '------------------------------------'
    )
    print(*init, sep='\n')
    while True:
        sel = input('Select a menu: ')
        if sel.isdigit() and int(sel) in {1, 2, 3}:
            return int(sel)
        print('Wrong Input!\n')


def play(score_list: list[list[str]] = [['x'] * 15 for _ in range(2)], turn: int = 1) -> None:
    os.system('cls')
    print('Starting a game...')
    while turn <= 12:
        print_score_board(score_list)
        print()
        reroll_count = 0
        player = roll_dice()
        print(f"[Player's Turn ({turn}/12)]")
        print(f'Roll: {player}')
        while reroll_count < 2:
            try:
                S = input('Which dice to reroll (1~5)? ')
                if S == 'Q':
                    save_data(score_list)
                    return
                reroll_indices = set(map(int, S.split()))
                if not reroll_indices:
                    break
                player = roll_dice(player, reroll_indices)
                print(f'Roll: {player}')
                reroll_count += 1
            except:
                print('Wrong input!')
        print()
        player.sort()
        print(f'[Sorted Roll] {player}')
        while True:
            categ = input('Choose a category: ').upper()
            if categ == 'Q':
                save_data(score_list)
                return
            if categ in CATEGORY:
                idx = CATEGORY[categ]
                if score_list[0][idx] == 'x':
                    break
            print('Wrong input!')
        print()
        score_list[0][idx] = str(calc_score(player, categ))
        add_bonus(score_list, 0)
        calc_total(score_list)

        print_score_board(score_list)
        computer = roll_dice()
        print(f"[Computer's Turn ({turn}/12)]")
        print(f'Roll: {computer}')
        for _ in range(2):
            reroll_indices = computer_pattern(computer, score_list)
            print('Which dice to reroll (1~5)?', *reroll_indices)
            if not reroll_indices:
                break
            computer = roll_dice(computer, reroll_indices)
            print(f'Roll: {computer}')
        print()
        categ, score = com_action(computer, score_list)
        computer.sort()
        print(f'Sorted Roll: {computer}')
        print(f'Choose a category: {categ}')
        print()
        idx = CATEGORY[categ]
        score_list[1][idx] = str(score)
        add_bonus(score_list, 1)
        calc_total(score_list)
        turn += 1

    print('<Final Score Board>')
    print_score_board(score_list)
    j = CATEGORY['T']
    if score_list[0][j] > score_list[1][j]:
        print('You win!')
    elif score_list[0][j] < score_list[1][j]:
        print('You lose!')
    else:
        print('Draw')
    print()
    input('Press Enter to continue...')


def save_data(score_list: list[list[str]]) -> None:
    fname = Path(input('Game paused. Enter the filename to save: '))
    file = fname.open('w')
    for categ, i in CATEGORY.items():
        if categ in {'sub', 'bonus', 'T'}:
            continue
        file.write(f'{categ}: {score_list[0][i]} {score_list[1][i]}\n')
    file.close()
    print('File saved.')
    time.sleep(0.5)


def calc_total(score_list: list[list[str]]) -> None:
    for i in range(2):
        total = 0
        for c, j in CATEGORY.items():
            if c in ['sub', 'T']:
                continue
            total = total + \
                int(score_list[i][j]) if score_list[i][j] != 'x' else total
        score_list[i][CATEGORY['T']] = str(total)
    return


def add_bonus(score_list: list[list[int]], who: int) -> None:
    bonus = True
    sub_total = 0
    for c in map(str, range(1, 7)):
        i = CATEGORY[c]
        try:
            sub_total += int(score_list[who][i])
        except:
            bonus = False
    score_list[who][CATEGORY['sub']] = str(sub_total)
    if bonus:
        score_list[who][CATEGORY['bonus']] = '35' if sub_total >= 63 else '0'
    return


def print_score_board(score_list: list[list[str]]) -> None:
    board: tuple[str, ...] = (
        '┌───────────────────────┬───────────────────────┐',
        '│         Player        │        Computer       │',
        '├───────────────────────┴───────────────────────┤',
        f'│  1:   {score_list[0][CATEGORY["1"]].replace("x", ""):>12}    │  1:   {score_list[1][CATEGORY["1"]].replace("x", ""):>12}    │',
        f'│  2:   {score_list[0][CATEGORY["2"]].replace("x", ""):>12}    │  2:   {score_list[1][CATEGORY["2"]].replace("x", ""):>12}    │',
        f'│  3:   {score_list[0][CATEGORY["3"]].replace("x", ""):>12}    │  3:   {score_list[1][CATEGORY["3"]].replace("x", ""):>12}    │',
        f'│  4:   {score_list[0][CATEGORY["4"]].replace("x", ""):>12}    │  4:   {score_list[1][CATEGORY["4"]].replace("x", ""):>12}    │',
        f'│  5:   {score_list[0][CATEGORY["5"]].replace("x", ""):>12}    │  5:   {score_list[1][CATEGORY["5"]].replace("x", ""):>12}    │',
        f'│  6:   {score_list[0][CATEGORY["6"]].replace("x", ""):>12}    │  6:   {score_list[1][CATEGORY["6"]].replace("x", ""):>12}    │',
        f'├───────────────────────────────────────────────┤',
        f'│  Sub total: {score_list[0][CATEGORY["sub"]].replace("x", "0"):>3}/63    │  Sub total: {score_list[1][CATEGORY["sub"]].replace("x", "0"):>3}/63    │',
        f'│  +35 bonus: {score_list[0][CATEGORY["bonus"]].replace("x", ""):>6}    │  +35 bonus: {score_list[1][CATEGORY["bonus"]].replace("x", ""):>6}    │',
        f'├───────────────────────────────────────────────┤',
        f'│  C:   {score_list[0][CATEGORY["C"]].replace("x", ""):>12}    │  C:   {score_list[1][CATEGORY["C"]].replace("x", ""):>12}    │',
        f'├───────────────────────────────────────────────┤',
        f'│  4K:  {score_list[0][CATEGORY["4K"]].replace("x", ""):>12}    │  4K:  {score_list[1][CATEGORY["4K"]].replace("x", ""):>12}    │',
        f'│  FH:  {score_list[0][CATEGORY["FH"]].replace("x", ""):>12}    │  FH:  {score_list[1][CATEGORY["FH"]].replace("x", ""):>12}    │',
        f'│  SS:  {score_list[0][CATEGORY["SS"]].replace("x", ""):>12}    │  SS:  {score_list[1][CATEGORY["SS"]].replace("x", ""):>12}    │',
        f'│  LS:  {score_list[0][CATEGORY["LS"]].replace("x", ""):>12}    │  LS:  {score_list[1][CATEGORY["LS"]].replace("x", ""):>12}    │',
        f'│  Yacht:   {score_list[0][CATEGORY["Y"]].replace("x", ""):>8}    │  Yacht:   {score_list[1][CATEGORY["Y"]].replace("x", ""):>8}    │',
        f'├───────────────────────────────────────────────┤',
        f'│  Total:   {score_list[0][CATEGORY["T"]].replace("x", "0"):>8}    │  Total:   {score_list[1][CATEGORY["T"]].replace("x", "0"):>8}    │',
        f'└───────────────────────────────────────────────┘\n'
    )
    print(*board, sep='\n')


def load_file2list() -> list[list[str]]:
    while True:
        file = Path(input('Enter filename to load: '))
        if not file.exists():
            print('File does not exist.\n')
            continue
        raw_data = file.read_text()
        score_list: list[list[str]] = [list() for _ in range(2)]
        if check_error(raw_data, score_list):
            print('Invalid file content.\n')
            continue
        break
    return score_list


def check_error(info: str, score_list: list[list[str]]) -> bool:
    lines = info.strip().split('\n')
    if len(lines) != 12:
        return True
    i = 0
    for row in lines:
        C = row.split()
        if i == CATEGORY['sub']:
            score_list[0].extend(['x', 'x'])
            score_list[1].extend(['x', 'x'])
            i += 2
        categ, s1, s2 = C
        categ = categ.rstrip(':')
        for s in [s1, s2]:
            if s.isdigit():
                k = int(s)
            else:
                continue
            if categ.isdigit():
                if k == 0:
                    continue
                if k % int(categ) != 0 or not 5 <= k <= k * 5:
                    return True
            elif categ in ['C', '4K', 'FH']:
                if k != 0 and not 5 <= k <= 30:
                    return True
            elif categ == 'SS':
                if k not in [0, 15]:
                    return True
            elif categ == 'LS':
                if k not in [0, 30]:
                    return True
            elif categ == 'Y':
                if k not in [0, 50]:
                    return True
        score_list[0].append(s1)
        score_list[1].append(s2)
        i += 1
    score_list[0].append('x')
    score_list[1].append('x')
    for i in range(2):
        add_bonus(score_list, i)
    calc_total(score_list)
    return False


def roll_dice(dice_set: list[int] = [], reroll_indices: set[int] = set()) -> list[int]:
    if not dice_set:
        dice_set = [random.randint(1, 6) for _ in range(5)]
    for i in reroll_indices:
        try:
            dice_set[i - 1] = random.randint(1, 6)
        except:
            pass
    return dice_set


def calc_score(dice_set: list[int], sel: str) -> int:
    count = Counter(dice_set)
    S = set(count.values())
    if sel.isdigit():
        num = int(sel)
        return count[num] * num
    elif sel == 'C':
        return sum(dice_set)
    elif sel == '4K':
        return sum(dice_set) if not {4, 5}.isdisjoint(S) else 0
    elif sel == 'FH':
        return sum(dice_set) if {3, 2}.issubset(S) or 5 in S else 0
    elif sel == 'SS':
        return 15 if is_cont(dice_set, 4) else 0
    elif sel == 'LS':
        return 30 if is_cont(dice_set, 5) else 0
    elif sel == 'Y':
        return 50 if 5 in S else 0


def is_cont(L: list[int], min_len: int) -> bool:
    ls = sorted(L)
    prev = ls[0]
    cnt = 1
    for i in range(1, len(ls)):
        if prev + 1 == ls[i]:
            cnt += 1
        elif prev == ls[i]:
            pass
        else:
            cnt = 1
        prev = ls[i]
    return True if cnt >= min_len else False


def computer_pattern(computer: list[int], score_list: list[list[str]]) -> set[int]:
    target, M = com_action(computer, score_list)
    if target.isdigit():
        reroll_indices = set(
            [i + 1 for i in range(5) if computer[i] != int(target)])
    elif target == 'C':
        reroll_indices = set([i + 1 for i in range(5) if computer[i] <= 3])
    else:
        reroll_indices = set([i + 1 for i in range(5)]) if M == 0 else set()
    return reroll_indices


def com_action(computer: list[int], score_list: list[list[str]]) -> tuple[str, int]:
    target, M = '', -1
    for categ, i in CATEGORY.items():
        if categ in ['sub', 'bonus', 'T']:
            continue
        if score_list[1][i] == 'x':
            score = calc_score(computer, categ)
            if score < 20 and categ == 'C':
                continue
            if M < score:
                M = score
                target = categ
    if not target:
        target = 'C'
        M = calc_score(computer, target)
    return target, M


if __name__ == '__main__':
    CATEGORY: dict[str, int] = {
        '1': 0,
        '2': 1,
        '3': 2,
        '4': 3,
        '5': 4,
        '6': 5,
        'sub': 6,
        'bonus': 7,
        'C': 8,
        '4K': 9,
        'FH': 10,
        'SS': 11,
        'LS': 12,
        'Y': 13,
        'T': 14
    }
    main()
