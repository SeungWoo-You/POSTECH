import tkinter as tk
import tkinter.font as font
from model_skeleton import Board

TITLE = "Minesweeper"
BTN_WIDTH = 30
BTN_HEIGHT = 30
BORDER_SIZE = 2
OUTTER_PADDING_SIZE = 10
BACKGROUND_COLOR = "#DCDCDC"
FONT_COLOR = {
    1: '#ff0000',
    2: '#ff8c00',
    3: '#f9bc28',
    4: '#008000',
    5: '#0000ff',
    6: '#4b0082',
    7: '$800080',
    8: '#000000'
}


class App(tk.Frame):
    def __init__(self, master: tk.Tk):
        self.master = master
        super().__init__(master)
        master.title(TITLE)
        master.resizable(False, False)
        self.width = 0
        self.height = 0
        self.numMine = 0
        self.board = Board()
        self.header: tk.Frame = tk.Frame(self.master)
        self.body: tk.Frame = tk.Frame(self.master)
        self.load_img()
        self.mk_easy()
        self.create_menu()
        self.btns: list[list[tk.Button]]

    def create_menu(self) -> None:
        menu_bar = tk.Menu(self.master)
        difficulty = tk.Menu(menu_bar, tearoff=0)
        menu_bar.add_cascade(label='난이도', menu=difficulty)
        difficulty.add_command(label='Easy', command=self.mk_easy)
        difficulty.add_command(label='Normal', command=self.mk_normal)
        difficulty.add_command(label='Hard', command=self.mk_hard)
        self.master.config(menu=menu_bar)

    def load_img(self) -> None:
        self.IMGs = {
            'smile': tk.PhotoImage(master=self.master, file='imgs/smile.png'),
            'empty': tk.PhotoImage(width=1, height=1),
            'flag': tk.PhotoImage(master=self.master, file='imgs/flag.png'),
            'mine': tk.PhotoImage(master=self.master, file='imgs/bomb.png'),
            'lose': tk.PhotoImage(master=self.master, file='imgs/skull.png'),
            'win': tk.PhotoImage(master=self.master, file='imgs/sunglasses.png')
        }

    def create_window(self) -> None:
        self.btns: list[list[tk.Button]] = []
        header = tk.Frame(self.master, borderwidth=BORDER_SIZE, relief='ridge')
        self.homebtn = tk.Button(
            header, command=self.reset, image=self.IMGs['smile'])
        self.homebtn.pack(side='top', fill=None)
        body = tk.Frame(self.master, padx=OUTTER_PADDING_SIZE,
                        borderwidth=BORDER_SIZE, relief='flat')
        for i in range(self.height):
            line: list[tk.Button] = []
            for j in range(self.width):
                btn = tk.Button(body, image=self.IMGs['empty'], width=BTN_WIDTH,
                                height=BTN_HEIGHT, compound='center')
                btn.grid(row=i, column=j)
                btn.bind('<Button-1>', self.click)
                btn.bind('<Button-3>', self.mark)
                line.append(btn)
            self.btns.append(line)
        header.pack(fill='both', expand=True)
        body.pack(fill='both')
        self.header = header
        self.body = body

    def mk_easy(self) -> None:
        self.set_status(10, 10, 10)

    def mk_normal(self) -> None:
        self.set_status(30, 15, 15)

    def mk_hard(self) -> None:
        self.set_status(50, 20, 20)

    def set_status(self, numMine: int, height: int, width: int) -> None:
        self.width = width
        self.height = height
        self.numMine = numMine
        self.reset()

    def reset(self) -> None:
        self.header.destroy()
        self.body.destroy()
        self.board.reset(self.numMine, self.height, self.width)
        self.create_window()

    def click(self, event: tk.Event) -> None:
        y, x = self.body.grid_location(event.x_root -
                                       self.body.winfo_rootx(), event.y_root - self.body.winfo_rooty())
        print('click: ', (x, y))
        ft_base = font.Font(size=13, weight='bold')
        if 0 <= x < self.height and 0 <= y < self.width and self.btns[x][y]['state'] != tk.DISABLED:
            res = self.board.unveil(x, y)
            if res == -1:
                for i, row in enumerate(self.btns):
                    for j, btn in enumerate(row):
                        btn.config(relief='groove', state='disabled')
                        if (i, j) in self.board.mines:
                            btn.config(image=self.IMGs['mine'])
                        else:
                            btn.config(image=self.IMGs['empty'])
                            numOfNearMines = self.board.getNumOfNearMines(i, j)
                            if numOfNearMines:
                                btn.config(
                                    text=numOfNearMines, font=ft_base, disabledforeground=FONT_COLOR[numOfNearMines])
                        btn.unbind('<Button-1>')
                        btn.unbind('<Button-3>')
                self.game_over(False)
            else:
                for i, row in enumerate(self.btns):
                    for j, btn in enumerate(row):
                        if self.board.checkReveal(i, j):
                            btn.config(relief='groove', state='disabled',
                                       image=self.IMGs['empty'])
                            numOfNearMines = self.board.getNumOfNearMines(i, j)
                            if numOfNearMines:
                                btn.config(
                                    text=numOfNearMines, font=ft_base, disabledforeground=FONT_COLOR[numOfNearMines])
                            btn.unbind('<Button-3>')
                if self.board.getNumOfRevealedPanels() == self.width * self.height - self.numMine:
                    for i, row in enumerate(self.btns):
                        for j, btn in enumerate(row):
                            btn.unbind('<Button-1>')
                            btn.unbind('<Button-3>')
                    self.game_over(True)

    def mark(self, event: tk.Event) -> None:
        y, x = self.body.grid_location(event.x_root -
                                       self.body.winfo_rootx(), event.y_root - self.body.winfo_rooty())
        print('mark: ', (x, y))
        self.board.toggleFlag(x, y)
        if self.board.checkFlag(x, y):
            self.btns[x][y].config(image=self.IMGs['flag'], state='disabled')
        else:
            self.btns[x][y].config(image=self.IMGs['empty'], state='active')

    def game_over(self, win: bool) -> None:
        if win:
            self.homebtn.config(image=self.IMGs['win'])
            print('clear!')
        else:
            self.homebtn.config(image=self.IMGs['lose'])
            print('boom!')


if __name__ == '__main__':
    root = tk.Tk()
    app = App(root)
    app.mainloop()
