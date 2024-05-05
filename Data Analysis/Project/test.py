import numpy as np


class Dummy:
    def __init__(self, amount) -> None:
        self.amount = amount

    def set_amount(self, x: int) -> None:
        self.amount = x

    def __repr__(self):
        return (
            f"(amount={self.amount})"
        )


X = np.zeros((7, 7), dtype=int)
Y = np.ones((3, 3), dtype=int)
for i in range(7 - 3 + 1):
    for j in range(7 - 3 + 1):
        X[i:i + 3, j: j + 3] += Y
D = np.vectorize(Dummy)
Z = D(amount=np.zeros((5, 5)))
print(X)
for i in range(5):
    for j in range(5):
        dum: Dummy = Z[i, j]
        dum.set_amount(X[i + 1, j + 1])
print(Z)
