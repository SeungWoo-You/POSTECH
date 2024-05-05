from __future__ import annotations
import os
import random


class Player:
    def __init__(self) -> None:
        self.mons: list[Posmon] = []
        self.status: list[str] = []
        self.cur_mon_idx = 0
        self.posmon: Posmon = None
        self.skill: Move | None = None

    def pick_mons(self, sel: int) -> None:
        if sel == 0:
            self.mons.append(Ponix())
        elif sel == 1:
            self.mons.append(Normie())
        elif sel == 2:
            self.mons.append(Swania())
        elif sel == 3:
            self.mons.append(Rocky())
        self.status.append('O')

    def show_mons(self) -> None:
        pass

    def show_info(self) -> None:
        pass

    def show_mon_status(self) -> None:
        mon = self.posmon
        print(f'{mon.name}   <|{mon.get_type()} {mon.health} / {mon.max_health}|')

    def turn(self) -> tuple[str, int]:
        pass

    def order(self, n: int) -> None:
        self.skill = self.posmon.moves[n]

    def switch_out(self, n: int) -> None:
        self.posmon.reset_status()
        self.cur_mon_idx = n
        self.posmon = self.mons[n]
        self.skill = None

    def excute(self, cmd: str) -> None:
        pass

    def is_dead(self) -> bool:
        pass


class User(Player):
    def __init__(self) -> None:
        super().__init__()

    def show_mons(self) -> None:
        print('당신의 포스몬 목록:', end=' ')
        for P in self.mons:
            print(P.name, end=' ')
        print()

    def show_info(self) -> None:
        print(
            f'당신의 포스몬: [{"".join(self.status)}] {self.status.count("O")} / {len(self.status)}')

    def show_skills(self) -> None:
        print(f'기술:', end=' ')
        mon = self.posmon
        for i in range(len(mon.moves)):
            print(f'({i}) {mon.moves[i].name}', end=' ')
        print()

    def turn(self) -> tuple[str, int]:
        while True:
            while True:
                action = input('입력: ')
                if action == 'e':
                    cmd = action
                    break
                try:
                    cmd, n = action.split()
                    n = int(n)
                    if cmd == 's':
                        if (not 0 <= n < len(self.mons)) or self.cur_mon_idx == n or self.status[n] == 'X':
                            print('포스몬을 교대시킬 수 없습니다!')
                        else:
                            break
                    elif cmd == 'o':
                        mon = self.mons[self.cur_mon_idx]
                        if not 0 <= n < len(mon.moves):
                            print('선택할 수 없는 기술입니다!')
                        else:
                            break
                    else:
                        print(f'잘못된 명령어: {action}')
                except:
                    print(f'잘못된 명령어: {action}')
            div()
            if cmd == 'e':
                self.entry()
                continue
            elif cmd == 'o':
                self.order(n)
                return (cmd, self.skill.get_speed())
            elif cmd == 's':
                self.switch_out(n)
                return (cmd, 4)

    def entry(self) -> None:
        for i in range(len(self.mons)):
            mon = self.mons[i]
            print(
                f'({i}) {mon.name}   <|{mon.get_type()} {mon.health} / {mon.max_health}|')
        print()

    def excute(self, cmd: str) -> None:
        if cmd == 's':
            print(f'당신의 포스몬 {self.posmon.name}로 교대')
        if cmd == 'o':
            print(f'당신의 {self.posmon.name}: {self.skill.name} 기술 사용')

    def is_dead(self) -> bool:
        if self.posmon.health <= 0:
            print(f'당신의 {self.posmon.name}: 쓰러짐')
            self.status[self.cur_mon_idx] = 'X'
            self.skill = None
            try:
                self.cur_mon_idx = self.status.index('O')
                self.posmon = self.mons[self.cur_mon_idx]
                print(f'당신의 포스몬 {self.posmon.name}로 교대')
            except:
                pass
            return True
        return False


class Com(Player):
    def __init__(self) -> None:
        super().__init__()

    def show_mons(self) -> None:
        print('컴퓨터 포스몬 목록:', end=' ')
        for P in self.mons:
            print(P.name, end=' ')
        print()

    def show_info(self) -> None:
        print(
            f'컴퓨터 포스몬: [{"".join(self.status)}] {self.status.count("O")} / {len(self.status)}')

    def turn(self, cmd: str = 'o') -> int:
        if cmd == 'o':
            i = self.cur_mon_idx
            mon = self.mons[i]
            n = random.randint(0, len(mon.moves) - 1)
            self.order(n)
            return self.skill.get_speed()

    def excute(self, cmd: str = 'o') -> None:
        if cmd == 'o':
            print(f'컴퓨터 {self.posmon.name}: {self.skill.name} 기술 사용')

    def is_dead(self) -> bool:
        if self.posmon.health <= 0:
            print(f'컴퓨터 {self.posmon.name}: 쓰러짐')
            self.status[self.cur_mon_idx] = 'X'
            self.skill = None
            try:
                self.cur_mon_idx = self.status.index('O')
                self.posmon = self.mons[self.cur_mon_idx]
                print(f'컴퓨터 포스몬 {self.posmon.name}로 교대')
            except:
                pass
            return True
        return False


class Game:
    def __init__(self) -> None:
        self.user: User = User()
        self.com: Com = Com()

    @property
    def line(self) -> None:
        print('============================================')

    @property
    def title(self) -> tuple[str, ...]:
        return (
            ' ____    ___    _____ ___ ___   ___   ____',
            '|    \  /   \  / ___/|   T   T /   \ |    \\',
            '|  o  )Y     Y(   \_ | _   _ |Y     Y|  _  Y',
            '|   _/ |  O  | \__  T|  \_/  ||  O  ||  |  |',
            '|  |   |     | /  \ ||   |   ||     ||  |  |',
            '|  |   l     ! \    ||   |   |l     !|  |  |',
            'l__j    \___/   \___jl___j___j \___/ l__j__j'
        )

    @property
    def mons_info(self) -> tuple[str, ...]:
        return (
            '0. Ponix',
            '1. Normie',
            '2. Swania',
            '3. Rocky'
        )

    def run(self) -> None:
        while True:
            self.show_title()
            sel = -1
            while True:
                try:
                    sel = int(input('입력: '))
                    if 0 <= sel <= 2:
                        break
                except:
                    pass
                print('잘못된 입력입니다. 다시 입력하세요.')
            print()
            if sel == 0:
                self.pick_posmon()
            elif sel == 1:
                self.battle()
                self.__init__()
            elif sel == 2:
                exit()

    def show_title(self) -> None:
        print(*self.title, sep='\n')
        self.line
        print('0. 포스몬 선택')
        print('1. 배틀하기')
        print('2. 종료하기')
        self.line

    def pick_posmon(self) -> None:
        while len(self.user.mons) < 3:
            N = len(self.user.mons)
            self.line
            print(f'당신이 사용할 포스몬을 선택하세요. 현재 {N} 마리/최대 3 마리')
            print(*self.mons_info, sep='\n')
            if N != 0:
                print('-1. 그만두기')
            self.line
            sel = -2
            while True:
                try:
                    sel = int(input('입력: '))
                    if 0 <= sel <= 3:
                        break
                    if N > 0 and sel == -1:
                        break
                except:
                    pass
                print('잘못된 입력입니다. 다시 입력하세요.')
            if sel == -1:
                break
            self.user.pick_mons(sel)
            print()
        self.line
        self.user.show_mons()
        self.line
        print('\n')

    def battle(self) -> None:
        if not self.user.mons:
            print('싸울 포스몬이 없습니다! 먼저 포스몬을 선택해 주세요.\n')
            return
        for sel in random.sample(range(4), 3):
            self.com.pick_mons(sel)
        self.user.cur_mon_idx = 0
        self.user.posmon = self.user.mons[0]
        self.com.cur_mon_idx = 0
        self.com.posmon = self.com.mons[0]
        self.line
        self.user.show_mons()
        self.com.show_mons()
        self.line
        print()
        print('배틀이 시작됩니다.')
        while True:
            div()
            self.com.show_info()
            self.com.show_mon_status()
            print('                     VS                     ')
            self.user.show_mon_status()
            self.user.show_info()
            print('++++++++++++++++++++++++++++++++++++++++++++')
            self.user.show_skills()
            div()
            if 'O' not in self.user.status:
                print()
                print('[배틀 결과] 컴퓨터가 이겼습니다!\n')
                return
            elif 'O' not in self.com.status:
                print()
                print('[배틀 결과] 당신이 이겼습니다!\n')
                return
            u_cmd, u_speed = self.user.turn()
            c_speed = self.com.turn()
            if u_speed >= c_speed:
                self.user.excute(u_cmd)
                if self.user.skill != None:
                    self.user.skill.use(
                        self.user.posmon, self.com.posmon, True)
                if not self.com.is_dead():
                    self.com.excute()
                    if self.com.skill != None:
                        self.com.skill.use(
                            self.user.posmon, self.com.posmon, False)
                        self.user.is_dead()
            else:
                self.com.excute()
                if self.com.skill != None:
                    self.com.skill.use(
                        self.user.posmon, self.com.posmon, False)
                if not self.user.is_dead():
                    self.user.excute(u_cmd)
                    if self.user.skill != None:
                        self.user.skill.use(
                            self.user.posmon, self.com.posmon, True)
                        self.com.is_dead()
            print()


class Posmon:
    def __init__(self, max_health: int, attack: int, defence: int, name: int, moves: list[Move]) -> None:
        self.health: int = max_health
        self.max_health: int = max_health
        self.attack: int = attack
        self.defence: int = defence
        self.moves: list[Move] = moves
        self.name: str = name

    def get_name(self) -> str:
        return self.name

    def get_max_health(self) -> int:
        return self.max_health

    def get_type(self) -> str:
        pass

    def reset_status(self) -> None:
        h = self.health
        self.__init__()
        self.health = h


class Ponix(Posmon):
    def __init__(self) -> None:
        super().__init__(86, 20, 23, 'Ponix', [
            Tackle(), Growl(), SwordDance()])

    def get_type(self) -> str:
        return 'Paper'


class Normie(Posmon):
    def __init__(self) -> None:
        super().__init__(80, 20, 20, 'Normie', [Tackle(), Swift(), TailWhip()])

    def get_type(self) -> str:
        return 'Nothing'


class Rocky(Posmon):
    def __init__(self) -> None:
        super().__init__(80, 15, 25, 'Rocky', [Tackle(), Growl()])

    def get_type(self) -> str:
        return 'Rock'


class Swania(Posmon):
    def __init__(self) -> None:
        super().__init__(80, 30, 10, 'Swania', [ScissorsCross(), SwordDance()])

    def get_type(self) -> str:
        return 'Scissors'


class Move:
    def __init__(self, name: str):
        self.name: str = name

    def get_name(self) -> str:
        return self.name

    def get_speed(self) -> int:
        pass

    def use(self, our_posmon: Posmon, opponent_posmon: Posmon, is_player_move=True):
        pass


class PhysicalMove(Move):
    def __init__(self, power: int, name: str):
        super().__init__(name)
        self.power: int = power

    def get_power(self) -> int:
        pass

    def use(self, our_posmon: Posmon, opponent_posmon: Posmon, is_player_move=True) -> None:
        hands = {
            'Rock': 0,
            'Scissors': 1,
            'Paper': 2
        }
        if is_player_move:
            h = opponent_posmon.health
            t1, t2 = our_posmon.get_type(), opponent_posmon.get_type()
            scale = 2 if t1 != 'Nothing' and t2 != 'Nothing' and (
                hands[t1] + 1) % 3 == hands[t2] else 1
            damage = max(0, self.power + our_posmon.attack -
                         opponent_posmon.defence) * scale
            opponent_posmon.health = h - damage
            print(
                f"- 컴퓨터 포스몬의 [체력] {damage} 감소 ({h}->{opponent_posmon.health})")
        else:
            h = our_posmon.health
            t1, t2 = opponent_posmon.get_type(), our_posmon.get_type()
            scale = 2 if t1 != 'Nothing' and t2 != 'Nothing' and (
                hands[t1] + 1) % 3 == hands[t2] else 1
            damage = max(0, self.power + opponent_posmon.attack -
                         our_posmon.defence) * scale
            our_posmon.health = h - damage
            print(
                f"- 당신 포스몬의 [체력] {damage} 감소 ({h}->{our_posmon.health})")


class Tackle(PhysicalMove):
    def __init__(self):
        super().__init__(25, 'Tackle')

    def get_speed(self) -> int:
        return 0


class ScissorsCross(PhysicalMove):
    def __init__(self):
        super().__init__(30, 'ScissorsCross')

    def get_speed(self) -> int:
        return 0


class Swift(PhysicalMove):
    def __init__(self):
        super().__init__(0, 'Swift')

    def get_speed(self) -> int:
        return 3


class StatusMove(Move):
    pass


class Growl(StatusMove):
    def __init__(self):
        super().__init__('Growl')
        self.amount = -5

    def get_speed(self) -> int:
        return 1

    def use(self, our_posmon: Posmon, opponent_posmon: Posmon, is_player_move=True) -> None:
        if is_player_move:
            d = opponent_posmon.attack
            opponent_posmon.attack = max(0, d + self.amount)
            print(
                f"- 컴퓨터 포스몬의 [공격력] {-self.amount} 감소 ({d}->{opponent_posmon.attack})")
        else:
            d = our_posmon.attack
            our_posmon.attack = max(0, d + self.amount)
            print(
                f"- 당신 포스몬의 [공격력] {-self.amount} 감소 ({d}->{our_posmon.attack})")


class SwordDance(StatusMove):
    def __init__(self):
        super().__init__('SwordDance')
        self.amount = 10

    def get_speed(self) -> int:
        return 0

    def use(self, our_posmon: Posmon, opponent_posmon: Posmon, is_player_move=True) -> None:
        if is_player_move:
            d = our_posmon.attack
            our_posmon.attack = max(0, d + self.amount)
            print(
                f"- 당신 포스몬의 [공격력] {self.amount} 증가 ({d}->{our_posmon.attack})")
        else:
            d = opponent_posmon.attack
            opponent_posmon.attack = max(0, d + self.amount)
            print(
                f"- 컴퓨터 포스몬의 [공격력] {self.amount} 증가 ({d}->{opponent_posmon.attack})")


class TailWhip(StatusMove):
    def __init__(self):
        super().__init__('TailWhip')
        self.amount = -5

    def get_speed(self) -> int:
        return 1

    def use(self, our_posmon: Posmon, opponent_posmon: Posmon, is_player_move=True) -> None:
        if is_player_move:
            d = opponent_posmon.defence
            opponent_posmon.defence = max(0, d + self.amount)
            print(
                f"- 컴퓨터 포스몬의 [방어력] {-self.amount} 감소 ({d}->{opponent_posmon.defence})")
        else:
            d = our_posmon.defence
            our_posmon.defence = max(0, d + self.amount)
            print(
                f"- 당신 포스몬의 [방어력] {-self.amount} 감소 ({d}->{our_posmon.defence})")


def main():
    game = Game()
    game.run()


def div() -> None:
    print('############################################')


if __name__ == '__main__':
    os.system('cls')
    main()
