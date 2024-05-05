import random


class Panel:
    def __init__(self) -> None:
        self.isRevealed = False
        self.hasFlag = False

    def toggleFlag(self) -> None:
        self.hasFlag = not self.hasFlag

    def unveil(self):
        self.isRevealed = True


class EmptyPanel(Panel):
    def __init__(self) -> None:
        super().__init__()
        self.numOfNearMines = 0

    def addNumOfNearMines(self):
        self.numOfNearMines += 1

    def unveil(self) -> int:
        super().unveil()
        return self.numOfNearMines


class MinePanel(Panel):
    def __init__(self) -> None:
        super().__init__()

    def unveil(self) -> int:
        super().unveil()
        return -1


class Board:
    def reset(self, numMine: int, height: int, width: int) -> None:
        self.numMine = numMine
        self.height = height
        self.width = width
        self.mines: set[tuple[int, int]] = set()
        self.panels: list[list[EmptyPanel | MinePanel]] = []
        self.numOfRevealedPanels = 0
        while len(self.mines) < numMine:
            x = random.randint(0, height - 1)
            y = random.randint(0, width - 1)
            if (x, y) not in self.mines:
                self.mines.add((x, y))

        Dx = [-1, 0, 1, -1, 1, -1, 0, 1]
        Dy = [-1, -1, -1, 0, 0, 1, 1, 1]
        for i in range(height):
            line: list[Panel] = []
            for j in range(width):
                if (i, j) in self.mines:
                    panel = MinePanel()
                else:
                    panel = EmptyPanel()
                    for dx, dy in zip(Dx, Dy):
                        x, y = i + dx, j + dy
                        if (x, y) in self.mines:
                            panel.addNumOfNearMines()
                line.append(panel)
            self.panels.append(line)

    def getNumOfRevealedPanels(self) -> int:
        return self.numOfRevealedPanels

    def unveil(self, x: int, y: int) -> int | None:
        if self.checkMine(x, y):
            self.numOfRevealedPanels += 1
            return self.panels[x][y].unveil()
        S: list[tuple[int, int]] = [(x, y)]
        Dx = [-1, 0, 1, -1, 1, -1, 0, 1]
        Dy = [-1, -1, -1, 0, 0, 1, 1, 1]
        while S:
            i, j = S.pop()
            panel = self.panels[i][j]
            if self.checkReveal(i, j):
                continue
            if self.checkFlag(i, j):
                self.toggleFlag(i, j)
            numOfNearMines = panel.unveil()
            self.numOfRevealedPanels += 1
            if (not self.checkMine(i, j)) and numOfNearMines == 0:
                for dx, dy in zip(Dx, Dy):
                    X, Y = i + dx, j + dy
                    if 0 <= X < self.height and 0 <= Y < self.width:
                        S.append((X, Y))
        return

    def toggleFlag(self, x: int, y: int) -> None:
        self.panels[x][y].toggleFlag()

    def checkReveal(self, x: int, y: int) -> bool:
        return self.panels[x][y].isRevealed

    def checkFlag(self, x: int, y: int) -> bool:
        return self.panels[x][y].hasFlag

    def checkMine(self, x: int, y: int) -> bool:
        return isinstance(self.panels[x][y], MinePanel)

    def getNumOfNearMines(self, x: int, y: int) -> int:
        return self.panels[x][y].numOfNearMines
