import os
import random


def main():
    word_list = ['apple', 'april', 'banana', 'blue', 'coral',
                 'dictionary', 'flower', 'peach', 'strawberry', 'watermelon']
    print("Hangman game starts!")
    while True:
        ans = play_game(word_list)
        if ans == 'no':
            break
    print()
    print("Quit the Hangman Game.")


def play_game(word_list: list[str]) -> bool:
    word = random.choice(word_list)
    used: list[str] = []
    life = 10

    while life > 0 and not is_word_guessed(word, used):
        char = select_char(word, used, life)
        if char in used:
            print("You have already checked this character. Try another one.")
            continue
        used.append(char)
        if char not in word:
            life -= 1

    if life > 0:
        print_status(word, used, life)
        print()
        print("Hangman Survived!")
    else:
        print("Hangman Die!")
        print(f"The answer was {word}.")

    while True:
        ans = input("Do you want to play another game?: ")
        if ans == 'yes' or ans == 'no':
            return ans
        print("Wrong input!")


def print_status(comp_word: str, used: list[str], life: int) -> None:
    found = reveal_word(comp_word, used)
    print("---------------------------------------------")
    print(' '.join(["Word:", *found]))
    print(' '.join(["Used:", *used]))
    print(f"Life: {life}")
    print("---------------------------------------------")
    return


def select_char(word: str, used: list[str], life: int) -> str:
    print_status(word, used, life)
    char = input("Choose a character: ")
    print()
    return char


def reveal_word(comp_word: str, used: list[str]) -> str:
    found = list('_' * len(comp_word))
    for char in used:
        for i, w in enumerate(comp_word):
            if char == w:
                found[i] = w
    return ''.join(found)


def is_word_guessed(comp_word: str, used: list[str]) -> bool:
    found = reveal_word(comp_word, used)
    if found == comp_word:
        return True
    return False


if __name__ == '__main__':
    os.system("cls")
    main()
