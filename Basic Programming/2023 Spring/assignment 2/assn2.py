import random
from pathlib import Path


def main():
    puzzle, N, blank_pos = init_puzzle_random()
    while True:
        mode = input("Enter the command ... (h: help, q: quit): ")
        if mode in ['w', 'a', 's', 'd']:
            blank_pos = move_blank_block(puzzle, N, mode, blank_pos)
            print()
            print_puzzle(puzzle, N)
        elif mode == 'c':
            use_chance(puzzle, N)
            print()
            print_puzzle(puzzle, N)
        elif mode == 'n':
            puzzle, N, blank_pos = init_puzzle_random()
        elif mode == 'f':
            puzzle, N, blank_pos = init_puzzle_file()
        elif mode == 'h':
            print_help()
        elif mode == 'q':
            print()
            print("Quit the puzzle game.")
            break
        else:
            print("Wrong input!")
            print()
        if is_solved(puzzle, N):
            print("The puzzle is solved.")
            break


def print_puzzle(puzzle: list[list[int | str]], size: int) -> None:
    for row in puzzle:
        print('+----' * size + '+')
        for x in row:
            if isinstance(x, str):
                print(f'|{x:>3s} ', end='')
            else:
                print(f'|{x:3d} ', end='')
        print('|')
    print('+----' * size + '+')


def find_blank(puzzle: list[list[int | str]]) -> list[int, int]:
    for i, row in enumerate(puzzle):
        for j, x in enumerate(row):
            if x == '*':
                return [i, j]
    raise Exception("Not Exist Blank-block.")


def swap_blocks(puzzle: list[list[int | str]], block_a: list[int, int], block_b: list[int, int]) -> None:
    x_a, y_a = block_a
    x_b, y_b = block_b
    puzzle[x_a][y_a], puzzle[x_b][y_b] = puzzle[x_b][y_b], puzzle[x_a][y_a]


def move_blank_block(puzzle: list[list[int | str]], size: int, command: str, blank: list[int, int]) -> list[int, int]:
    move = {
        'w': [-1, 0],
        's': [1, 0],
        'a': [0, -1],
        'd': [0, 1]
    }

    x, y = map(sum, zip(blank, move[command]))
    if 0 <= x < size and 0 <= y < size:
        swap_blocks(puzzle, blank, [x, y])
        blank = [x, y]
    else:
        print("Can not move the *")
    return blank


def create_random_puzzle(size: int) -> list[list[int | str]]:
    blocks = list(range(1, size**2)) + ['*']
    random.shuffle(blocks)
    puzzle = [
        [x for x in blocks[size * i:size * (i + 1)]] for i in range(size)]
    return puzzle


def create_from_file(filename: Path) -> tuple[list[list[int | str]], int]:
    data = filename.read_text().split('\n')
    N = int(data[0])
    puzzle: list[list[int | str]] = []
    for line in data[1:]:
        if not line:
            break
        try:
            puzzle.append(list(map(int, line.split())))
        except:
            pre, post = line.split('*')
            row = list(map(int, pre.split())) + \
                ['*'] + list(map(int, post.split()))
            puzzle.append(row)
    return puzzle, N


def is_valid(puzzle: list[list[int | str]], size: int) -> bool:
    blocks = dict()
    star = False
    for i, row in enumerate(puzzle):
        for j, x in enumerate(row):
            if x == '*':
                star = True
                continue
            if x < 1 or x > size**2 - 1:
                return False
            if x in blocks:
                return False
            blocks[x] = 1
        if j != size - 1:
            return False
    if i != size - 1 or star == False:
        return False
    return True


def is_solved(puzzle: list[list[int | str]], N: int) -> bool:
    for i, row in enumerate(puzzle):
        for j, x in enumerate(row):
            if x == '*':
                if i == N - 1 and j == N - 1:
                    return True
                else:
                    return False
            elif x != i * N + j:
                return False
    return True


def init_puzzle_random() -> tuple[list[list[int | str]], int, list[int, int]]:
    while True:
        N = int(input("Choose the puzzle size: "))
        if 3 <= N <= 6:
            break
        print("Wrong size!")

    puzzle = create_random_puzzle(N)
    print("Enjoy the puzzle!")
    print_puzzle(puzzle, N)
    blank_pos = find_blank(puzzle)
    return puzzle, N, blank_pos


def init_puzzle_file() -> tuple[list[list[int | str]], int, list[int, int]]:
    while True:
        fname = input("File Name: ")
        path = Path(fname)
        if not path.is_file():
            print("No file is found")
            continue
        puzzle, N = create_from_file(path)
        if is_valid(puzzle, N):
            break
        print("Error!")
    print("Enjoy the puzzle!")
    print_puzzle(puzzle, N)
    blank_pos = find_blank(puzzle)
    return puzzle, N, blank_pos


def print_help() -> None:
    print('w: Move * to UPPER bolck')
    print('s: Move * to LOWER bolck')
    print('a: Move * to LEFT bolck')
    print('d: Move * to RIGHT bolck')
    print('c: use CHANCE')
    print('n: Create NEW puzzle (randomly generated puzzle)')
    print('f: Create NEW puzzle (import puzzle from txt file)')
    print('h: Print help message')
    print('q: Quit the puzzle')
    print()


def use_chance(puzzle: list[list[int | str]], size: int) -> None:
    while True:
        try:
            A, B = map(list, input("Use CHANCE! Choose the blocks:").split())
            if not (len(A) == len(B) == 2):
                raise Exception
            pos = list(map(int, A + B))
            res = [x for x in pos if 0 <= x < size]
            if len(res) == 4:
                break
            raise Exception
        except:
            print("Wrong input!")
    swap_blocks(puzzle, pos[:2], pos[2:])


if __name__ == '__main__':
    main()
