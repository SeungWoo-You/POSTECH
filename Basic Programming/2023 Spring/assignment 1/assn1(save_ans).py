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
    found = '_' * len(word)
    used: list[str] = []
    life = 10

    while life > 0 and not is_word_guessed(word, found):
        char = select_char(found, used, life)
        if char in used:
            print("You have already checked this character. Try another one.")
            continue
        used.append(char)
        temp_found = reveal_word(word, found, char)
        if temp_found == found:
            life -= 1
        else:
            found = temp_found

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
    print("---------------------------------------------")
    print(' '.join(["Word:", *comp_word]))
    print(' '.join(["Used:", *used]))
    print(f"Life: {life}")
    print("---------------------------------------------")
    return


def select_char(word: str, used: list[str], life: int) -> str:
    print_status(word, used, life)
    char = input("Choose a character: ")
    print()
    return char


def reveal_word(comp_word: str, found: str, char: str) -> str:
    for i, ch in enumerate(comp_word):
        if ch == char:
            found = ''.join([found[:i], ch, found[i + 1:]])
    return found


def is_word_guessed(comp_word: str, found: str) -> bool:
    if comp_word == found:
        return True
    else:
        return False


if __name__ == '__main__':
    os.system("cls")
    main()
