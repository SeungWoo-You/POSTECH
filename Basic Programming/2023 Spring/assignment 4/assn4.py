import tkinter as tk
import random


class GameObject:
    def __init__(self, canvas: tk.Canvas, item: int) -> None:
        self.canvas = canvas
        self.item = item

    def get_position(self) -> list[float]:
        return self.canvas.coords(self.item)

    def delete(self) -> None:
        self.canvas.delete(self.item)

    def move(self, x: int, y: int) -> None:
        self.canvas.move(self.item, x, y)


class Quadrangle(GameObject):
    def __init__(self, canvas: tk.Canvas, x: int, y: int, width: int, height: int, color: str) -> None:
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.color = color
        item = canvas.create_rectangle(
            x - width / 2, y - height / 2,
            x + width / 2, y + height / 2,
            fill=color
        )
        super().__init__(canvas, item)


class Player(Quadrangle):
    def __init__(self, canvas: tk.Canvas, x: int, y: int) -> None:
        self.state = 0
        self.x = x
        self.y = y
        super().__init__(canvas, x, y, 50, 50, 'blue')

    def check_collision(self, tPos: list[float]) -> bool:
        x1, y1, x2, y2 = tPos
        l, t, r, b = self.get_position()
        if r > x1 and x2 > l:
            if t < y2 and y1 < b:
                return True
        return False


class Terrain(Quadrangle):
    def __init__(self, canvas: tk.Canvas, x: int, y: int) -> None:
        self.x = x
        self.y = y
        super().__init__(canvas, x, y, 250, 200, 'green')


class Circle(GameObject):
    def __init__(self, canvas: tk.Canvas, x: int, y: int, radius: int, color: str) -> None:
        self.x = x
        self.y = y
        self.radius = radius
        self.color = color
        item = canvas.create_oval(
            x - radius, y - radius,
            x + radius, y + radius,
            fill=color
        )
        super().__init__(canvas, item)


class Coin(Circle):
    def __init__(self, canvas: tk.Canvas, x: int, y: int) -> None:
        self.x = x
        self.y = y
        super().__init__(canvas, x, y, 10, 'yellow')


class FireBall(Circle):
    def __init__(self, canvas: tk.Canvas, x: int, y: int) -> None:
        self.x = x
        self.y = y
        super().__init__(canvas, x, y, 15, 'red')


class Game(tk.Frame):
    def __init__(self, master: tk.Tk, width: int, height: int) -> None:
        super(Game, self).__init__(master)
        self.width = width
        self.height = height
        self.canvas = tk.Canvas(
            self, bg='white', width=self.width, height=self.height)
        self.canvas.pack(fill="both", expand=True)
        self.pack()

        self.setup_game()

    def setup_game(self) -> None:
        self.gameInit()
        self.canvas.bind('<space>', lambda _: self.update_jumping())
        self.canvas.focus_set()
        self.game_loop()

    def gameInit(self):
        self.end_font = ('Helvetica', '40')
        self.score_font = ('Helvetica', '15')

        self.score = 0
        self.mapSpeed = -5
        self.jumpSpeed = 0

        self.terrains = [Terrain(self.canvas, i * 250, 600) for i in range(7)]
        self.fireballs = [FireBall(self.canvas, 1200, self.random_posY())]
        self.coins = [Coin(self.canvas, 1200, self.random_posY())]
        self.player = Player(self.canvas, 200, 475)
        self.score_text = self.canvas.create_text(
            1100, 50, text=f'SCORE: {self.score}', font=self.score_font)
        self.end_text = self.canvas.create_text(
            self.width / 2, self.height / 2, text='', font=self.end_font)

    def game_loop(self):
        if (self.player.state != 2):
            self.gameSystem()
            self.after(10, self.game_loop)

    def gameSystem(self):
        self.move_map()
        self.manage_map()
        self.check_gameState()

    def manage_map(self):
        self.manage_terrains()
        self.manage_fireballs()
        self.manage_coins()

    def manage_terrains(self) -> None:
        tr = self.terrains[0]
        pos = tr.get_position()
        if pos[2] < 0:
            tr.delete()
            self.terrains.pop(0)
            last_pos = self.terrains[-1].get_position()
            new_tr = Terrain(
                self.canvas, last_pos[2] + 250 / 2, self.terrains[-1].y)
            self.terrains.append(new_tr)

    def manage_fireballs(self):
        fb = self.fireballs[0]
        pos = fb.get_position()
        if self.player.check_collision(pos) == True:
            self.player.state = 2
            return
        if pos[0] < 0:
            fb.delete()
            self.fireballs.pop(0)
            new_fb = FireBall(self.canvas, 1200, self.random_posY())
            self.fireballs.append(new_fb)

    def manage_coins(self) -> None:
        c = self.coins[0]
        pos = c.get_position()
        regen = False

        if self.player.check_collision(pos) == True:
            self.score += 1
            self.canvas.itemconfig(
                self.score_text, text=f'SCORE: {self.score}')
            regen = True
        if pos[0] < 0:
            regen = True

        if regen == True:
            c.delete()
            self.coins.pop(0)
            new_c = Coin(self.canvas, 1200, self.random_posY())
            self.coins.append(new_c)

    def random_posY(self) -> int:
        num = random.randint(0, 1)
        if (num == 0):
            return 400
        else:
            return 450

    def check_gameState(self) -> None:
        if (self.player.state == 2):
            self.canvas.itemconfig(self.end_text, text="END")
        elif (self.player.state == 1):
            self.jumping()

    def update_jumping(self) -> None:
        if self.player.state == 0:
            self.jumpSpeed = -15
            self.player.state = 1

    def jumping(self):
        grav = 1
        self.player.move(0, self.jumpSpeed)
        if self.jumpSpeed == 15:
            self.player.state = 0
        self.jumpSpeed += grav

    def move_map(self) -> None:
        for tr in self.terrains:
            tr.move(self.mapSpeed, 0)
        for c in self.coins:
            c.move(self.mapSpeed, 0)
        for fb in self.fireballs:
            fb.move(2 * self.mapSpeed, 0)


if __name__ == '__main__':
    root = tk.Tk()
    root.title('Assignment4_ID')
    game = Game(root, 1200, 700)
    game.mainloop()
