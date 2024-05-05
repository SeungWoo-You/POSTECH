import os
from collections import Counter
import random
from pathlib import Path
import time


class User:
    turn = 1

    def __init__(self, name: str) -> None:
        User.turn = 1
        self.name = name
        self.scores: dict[str, int] = {
            '1': -1,
            '2': -1,
            '3': -1,
            '4': -1,
            '5': -1,
            '6': -1,
            'C': -1,
            '4K': -1,
            'FH': -1,
            'SS': -1,
            'LS': -1,
            'Y': -1
        }
        self.sub_total = 0
        self.bonus = -1
        self.total = 0
        self.dice: list[int] = [0] * 5
        self.checked: set[str] = set()

    def play(self) -> bool:
        print(f"{self.name}'s Turn ({User.turn}/12)")
        self.roll_dice()
        print(f'Roll: {self.dice}')
        for _ in range(2):
            result = self.strategize()
            if result == 0:
                break
            elif result == -1:
                return False
            print(f'Roll: {self.dice}')
        self.dice.sort()
        print(f'\nSorted Roll: {self.dice}')
        if not self.categorize():
            return False
        self.get_bonus()
        return True

    def strategize(self) -> int:
        pass

    def categorize(self) -> None:
        pass

    def roll_dice(self, indices: set[int] = set(range(1, 6))) -> None:
        for i in indices:
            try:
                self.dice[i - 1] = random.randint(1, 6)
            except:
                pass
        return

    def put_score(self, categ: str, point: int) -> None:
        self.scores[categ] = point
        if categ.isdigit():
            self.sub_total += point
        self.total += point
        self.checked.add(categ)
        return

    def calc_score(self, categ: str) -> int:
        count = Counter(self.dice)
        S = set(count.values())
        if categ.isdigit():
            num = int(categ)
            return count[num] * num
        elif categ == 'C':
            return sum(self.dice)
        elif categ == '4K':
            return sum(self.dice) if not {4, 5}.isdisjoint(S) else 0
        elif categ == 'FH':
            return sum(self.dice) if {3, 2}.issubset(S) or 5 in S else 0
        elif categ == 'SS':
            return 15 if self.is_cont(4) else 0
        elif categ == 'LS':
            return 30 if self.is_cont(5) else 0
        elif categ == 'Y':
            return 50 if 5 in S else 0

    def is_cont(self, min_len: int) -> bool:
        ls = sorted(self.dice)
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

    def get_bonus(self) -> None:
        if self.bonus != -1:
            return
        if self.sub_total >= 63:
            self.bonus = 35
            self.total += 35
            return
        end = True if self.checked.issuperset(map(str, range(1, 7))) else False
        if end:
            self.bonus = 0
        return


class Player(User):
    def __init__(self) -> None:
        super().__init__('Player')

    def strategize(self) -> int:
        while True:
            select = input('Which dice to reroll (1~5)? ')
            if select == 'Q':
                return -1
            try:
                indices = set(map(int, select.split()))
                break
            except:
                print('Wrong input!')
        self.roll_dice(indices)
        return 1 if indices else 0

    def categorize(self) -> bool:
        while True:
            categ = input('Choose a category: ').upper()
            if categ == 'Q':
                return False
            if categ in self.scores and self.scores[categ] == -1:
                point = self.calc_score(categ)
                self.put_score(categ, point)
                break
            else:
                print('Wrong input!')
        return True


class Com(User):
    def __init__(self) -> None:
        super().__init__('Computer')

    def choose_category(self) -> str:
        target, M = '', -1
        for categ, pt in self.scores.items():
            if pt != -1:
                continue
            point = self.calc_score(categ)
            if categ == 'C' and point < 20:
                continue
            if M < point:
                M = point
                target = categ
        if not target:
            target = 'C'
        return target

    def strategize(self) -> int:
        target = self.choose_category()
        if target.isdigit():
            indices = set(
                [i + 1 for i in range(5) if self.dice[i] != int(target)])
        elif target == 'C':
            indices = set([i + 1 for i in range(5) if self.dice[i] <= 3])
        else:
            point = self.calc_score(target)
            indices = set([i + 1 for i in range(5)]) if point == 0 else set()
        print('Which dice to reroll (1~5)?', *indices)
        self.roll_dice(indices)
        return 1 if indices else 0

    def categorize(self) -> bool:
        categ = self.choose_category()
        print(f'Choose a category: {categ}')
        point = self.calc_score(categ)
        self.put_score(categ, point)
        return True


class Game:
    def __init__(self) -> None:
        os.system('cls')
        self.player: Player = Player()
        self.com: Com = Com()

    def show_scores(self) -> None:
        player = self.player
        com = self.com
        board: tuple[str, ...] = (
            '┌───────────────────────┬───────────────────────┐',
            '│         Player        │        Computer       │',
            '├───────────────────────┴───────────────────────┤',
            f'│  1:   {player.scores["1"] if player.scores["1"] != -1 else "":>12}    │  1:   {com.scores["1"] if com.scores["1"] != -1 else "":>12}    │',
            f'│  2:   {player.scores["2"] if player.scores["2"] != -1 else "":>12}    │  2:   {com.scores["2"] if com.scores["2"] != -1 else "":>12}    │',
            f'│  3:   {player.scores["3"] if player.scores["3"] != -1 else "":>12}    │  3:   {com.scores["3"] if com.scores["3"] != -1 else "":>12}    │',
            f'│  4:   {player.scores["4"] if player.scores["4"] != -1 else "":>12}    │  4:   {com.scores["4"] if com.scores["4"] != -1 else "":>12}    │',
            f'│  5:   {player.scores["5"] if player.scores["5"] != -1 else "":>12}    │  5:   {com.scores["5"] if com.scores["5"] != -1 else "":>12}    │',
            f'│  6:   {player.scores["6"] if player.scores["6"] != -1 else "":>12}    │  6:   {com.scores["6"] if com.scores["6"] != -1 else "":>12}    │',
            f'├───────────────────────────────────────────────┤',
            f'│  Sub total: {player.sub_total:>3}/63    │  Sub total: {com.sub_total:>3}/63    │',
            f'│  +35 bonus: {player.bonus if player.bonus != -1 else "":>6}    │  +35 bonus: {com.bonus if com.bonus != -1 else "":>6}    │',
            f'├───────────────────────────────────────────────┤',
            f'│  C:   {player.scores["C"] if player.scores["C"] != -1 else "":>12}    │  C:   {com.scores["C"] if com.scores["C"] != -1 else "":>12}    │',
            f'├───────────────────────────────────────────────┤',
            f'│  4K:  {player.scores["4K"] if player.scores["4K"] != -1 else "":>12}    │  4K:  {com.scores["4K"] if com.scores["4K"] != -1 else "":>12}    │',
            f'│  FH:  {player.scores["FH"] if player.scores["FH"] != -1 else "":>12}    │  FH:  {com.scores["FH"] if com.scores["FH"] != -1 else "":>12}    │',
            f'│  SS:  {player.scores["SS"] if player.scores["SS"] != -1 else "":>12}    │  SS:  {com.scores["SS"] if com.scores["SS"] != -1 else "":>12}    │',
            f'│  LS:  {player.scores["LS"] if player.scores["LS"] != -1 else "":>12}    │  LS:  {com.scores["LS"] if com.scores["LS"] != -1 else "":>12}    │',
            f'│  Yacht:   {player.scores["Y"] if player.scores["Y"] != -1 else "":>8}    │  Yacht:   {com.scores["Y"] if com.scores["Y"] != -1 else "":>8}    │',
            f'├───────────────────────────────────────────────┤',
            f'│  Total:   {player.total:>8}    │  Total:   {com.total:>8}    │',
            f'└───────────────────────────────────────────────┘\n'
        )
        print(*board, sep='\n')

    def run(self) -> None:
        while User.turn <= 12:
            print()
            self.show_scores()
            if self.player.play() == False:
                self.pause()
                return
            print()
            self.show_scores()
            self.com.play()
            User.turn += 1
        print('<Final Score Board>')
        self.show_scores()
        if self.player.total > self.com.total:
            print('You win!')
        elif self.player.total < self.com.total:
            print('You lose!')
        else:
            print('Draw')
        print()
        input('Press Enter to continue...')

    def pause(self) -> None:
        fname = Path(input('Game paused. Enter the filename to save: '))
        file = fname.open('w')
        for categ in self.player.scores:
            p_pt = self.player.scores[categ] if self.player.scores[categ] != -1 else 'x'
            c_pt = self.com.scores[categ] if self.com.scores[categ] != -1 else 'x'
            file.write(f'{categ}: {p_pt} {c_pt}\n')
        file.close()
        print('File saved.')
        time.sleep(0.5)

    def load(self) -> None:
        while True:
            file = Path(input('Enter filename to load: '))
            if not file.exists():
                print('File does not exist.\n')
                continue
            raw_data = file.read_text()
            if not self.is_valid(raw_data):
                self.player = Player()
                self.com = Com()
                print('Invalid file content.\n')
                continue
            break
        return

    def is_valid(self, data: str) -> bool:
        lines = data.strip().split('\n')
        turn = 1
        if len(lines) != 12:
            return False
        for row in lines:
            categ, p_pt, c_pt = row.split()
            categ = categ.rstrip(':')
            for pt in [p_pt, c_pt]:
                if pt == 'x':
                    continue
                k = int(pt)
                if categ.isdigit():
                    if k == 0:
                        continue
                    if k % int(categ) != 0 or not 5 <= k <= k * 5:
                        return False
                elif categ in ['C', '4K', 'FH']:
                    if k != 0 and not 5 <= k <= 30:
                        return False
                elif categ == 'SS':
                    if k not in [0, 15]:
                        return False
                elif categ == 'LS':
                    if k not in [0, 30]:
                        return False
                elif categ == 'Y':
                    if k not in [0, 50]:
                        return False
            if p_pt != 'x':
                self.player.put_score(categ, int(p_pt))
                turn += 1
            if c_pt != 'x':
                self.com.put_score(categ, int(c_pt))
        self.player.get_bonus()
        self.com.get_bonus()
        User.turn = turn
        return True


def main():
    init: tuple[str, ...] = (
        '[Yacht Dice]',
        '------------------------------------',
        '1. New Game   2. Load Game   3. Exit',
        '------------------------------------'
    )
    while True:
        game = Game()
        print(*init, sep='\n')
        sel = select_menu()
        if sel == 1:
            game.run()
        elif sel == 2:
            game.load()
            game.run()
        if sel == 3:
            print('Program ended. Bye!')
            return


def select_menu() -> int:
    while True:
        sel = input('Select a menu: ')
        if sel.isdigit() and int(sel) in {1, 2, 3}:
            return int(sel)
        print('Wrong Input!\n')


if __name__ == '__main__':
    main()
