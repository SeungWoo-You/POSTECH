import random
from dataclasses import dataclass
import time


def main():
    S = Station()
    while True:
        select = S.default_screen()
        if select == 0:
            S.serve()
            S.today_num += 1
            S.total_num += 1
        elif select == 1:
            S.refill()
        elif select == 2:
            S.print_status()
        elif select == 3:
            if S.next() == True:
                S.day += 1
                S.today_num = 0
        elif select == 4:
            if S.end() == True:
                break


@dataclass
class Gas:
    name: str
    price: float
    remains: int


class Name:
    DIESEL = 'Diesel'
    GASOLINE = 'Gasoline'

    def __setattr__(self, name: str, value):
        raise Exception('Cannot add variable')

    def __delattr__(self, name: str):
        if name in self.__dict__:
            raise Exception('Cannot delete variable')


class Station:
    def __init__(self) -> None:
        self.day = 1
        self.rating = 0
        self.money = 1000.00
        self.today_num = 0
        self.total_num = 0
        self.customer: SUV | Hybrid | Bus | Truck | None = None
        self.gas = {
            Name.DIESEL: Gas(Name.DIESEL, 10.0, 100),
            Name.GASOLINE: Gas(Name.GASOLINE, 15.0, 100)
        }
        self.method: dict[str, str | int] = {
            'gas': Name.DIESEL,
            'amount': 10
        }

    @property
    def RATING_RESULT(self) -> dict[str, list[int | str]]:
        return {
            'for_free': [3, "Driver: I'm blessed to have all of these. Thanks!"],
            'let_go': [-1, "Owner: Currently, we are not available for that.\nDriver: Well, see you then!"],
            'good_job': [1, "Driver: Thanks a lot!"],
            'wrong_fuel': [-5, "System: This is not the right fuel type!"],
            'overflow': [-3, "Driver: Hey, it overflows! Stop there!"],
            'different': [-1, "Driver: Well, not the exact amount, but thanks anyway!"],
            'not_enough': [-1, "System: There is not enough fuel in the tank!"]
        }

    def state_update(self, status: str) -> None:
        rs = self.RATING_RESULT
        print(rs[status][1])

        if status in ['good_job', 'overflow', 'different']:
            curr_gas = self.method['gas']

            put_in = self.calc_sell_amount(status)
            print(
                f'Money: ${self.money:.2f} -> ${self.money + self.gas[curr_gas].price * put_in:.2f}')
            self.money += self.gas[curr_gas].price * put_in
            print(
                f'{curr_gas}: {self.gas[curr_gas].remains} Liters -> {self.gas[curr_gas].remains - put_in} Liters')
            self.gas[curr_gas].remains -= put_in

        if status == 'good_job':
            print(
                f'Rating changed: {self.rating} -> {self.rating + rs[status][0]}')
        else:
            print(f'Rating: {self.rating} -> {self.rating + rs[status][0]}')
        self.rating += rs[status][0]

    def calc_sell_amount(self, status: str) -> int:
        if status == 'good_job':
            return self.customer.needed

        curr_max = self.customer.capacity - self.customer.cur_fuel
        if status == 'overflow':
            return curr_max
        if status == 'different':
            return self.method['amount']
        else:
            raise Exception()

    def refill(self):
        print('Which one do you want to refill?')
        print(f'0. {Name.DIESEL}')
        print(f'1. {Name.GASOLINE}')
        select = int(input('Select: '))
        gas_name = Name.DIESEL if select == 0 else Name.GASOLINE
        discount_ratio = self.discount_rate()
        unit_price = self.gas[gas_name].price * 0.9
        buying_price = unit_price * (1 - discount_ratio)
        notice = (
            f"Based on your rating {self.rating}, the discount ratio is {discount_ratio * 100:.2f}%\n"
            f"The base unit buying price of {gas_name} for today is ${unit_price:.2f}, "
            f"so the discount unit buying price will be ${buying_price:.2f}"
        )
        print(notice)
        while True:
            print()
            liter = input(
                f"You have ${self.money:.2f}. Amount of {gas_name.lower() + 's'} to buy(liters): ")
            if liter.isdigit() and int(liter) > 0:
                amount = int(liter)
                cost = amount * buying_price
                if self.money - cost < 0:
                    print("You don't have enough money")
                    continue
                else:
                    print(
                        f'Money spent: ${self.money:.2f} -> ${self.money - cost:.2f}')
                    self.money -= cost
                    if select == 0:
                        diesel = self.gas[Name.DIESEL].remains
                        print(
                            f'{gas_name} refilled: {diesel} Liters -> {diesel + amount} Liters')
                        self.gas[Name.DIESEL].remains += amount
                    else:
                        gasoline = self.gas[Name.GASOLINE].remains
                        print(
                            f'{gas_name} refilled: {gasoline} Liters -> {gasoline + amount} Liters')
                        self.gas[Name.GASOLINE].remains += amount
                    break
            else:
                print('Canceled')
                break
        print()

    def print_status(self) -> None:
        print('---------STATUS---------')
        print(f'Day {self.day}')
        print(f'Rating: {self.rating}')
        print(f'Money: ${self.money:.2f}')
        print(f'# Customers handled for today: {self.today_num}')
        print(f'{Name.DIESEL} left: {self.gas[Name.DIESEL].remains} Liters')
        print(
            f'{Name.GASOLINE} left: {self.gas[Name.GASOLINE].remains} Liters')
        print('')

    def default_screen(self) -> int:
        print('---------GAS STATION---------')
        print('0. Wait for a vehicle')
        print('1. Refill tanks')
        print('2. Show current status')
        print('3. Go to the next day')
        print('4. End Game')

        while True:
            select = input('Select: ')
            if select.isdigit() and 0 <= int(select) <= 4:
                break
            print('\nWrong input!')
        print('')
        return int(select)

    def serve(self) -> None:
        print("Waiting...")
        print()
        time.sleep(1)

        self.choose_car()
        self.customer.printInfo()
        fuel_mode = random.randint(1, 5)
        if fuel_mode == 1:
            self.customer.upgrade_claim()
            print('0. Yes')
            print('1. No')
            select = int(input('Select: '))
            print()

            if select == 0:
                rs = self.RATING_RESULT
                status = 'for_free'
                cost = self.customer.upgrade()
                print(f'Money: ${self.money:.2f} -> ${self.money - cost:.2f}')
                self.money -= cost
                print(rs[status][1])
                print(
                    f'Rating: {self.rating} -> {self.rating + rs[status][0]}')
                self.rating += rs[status][0]
            else:
                cost = self.customer.upgrade()
                status = 'let_go'
                self.state_update(status)

        else:
            if self.customer.full == True:
                print("Driver: please make it full!")
            else:
                print(
                    f"Driver: I'd like {self.customer.needed} liters, please.")
                print(
                    f"{Name.DIESEL} left: {self.gas[Name.DIESEL].remains} Liters, {Name.GASOLINE} left: {self.gas[Name.GASOLINE].remains} Liters")
            print()

            while True:
                print("0. Change fueling method")
                print("1. Start fueling")
                print("2. Let go")
                select = int(input('Select: '))
                print()

                if select == 0:
                    self.change_method()
                    continue
                elif select == 1:
                    status = self.fuel()
                else:
                    status = 'let_go'
                self.state_update(status)
                break
        print()

    def price_update(self) -> None:
        self.gas[Name.GASOLINE].price *= (1 + random.uniform(-0.1, 0.1))
        self.gas[Name.DIESEL].price *= (1 + random.uniform(-0.1, 0.1))

    def next(self) -> bool:
        if self.today_num < 3:
            print(
                f"You have to handle at least three customers. ({self.today_num} / 3)\n")
            return False

        prev_prices = (self.gas[Name.GASOLINE].price,
                       self.gas[Name.DIESEL].price)
        self.price_update()
        print(f'Day {self.day} finished.')
        print(
            f'{Name.GASOLINE} unit selling price: ${prev_prices[0]:.2f} -> ${self.gas[Name.GASOLINE].price:.2f}')
        print(
            f'{Name.DIESEL} unit selling price: ${prev_prices[1]:.2f} -> ${self.gas[Name.DIESEL].price:.2f}')
        print()
        return True

    def discount_rate(self) -> float:
        return min(max(0, self.rating / 2), 30) / 100

    def choose_car(self) -> None:
        type = random.randint(1, 6)
        if type == 1:
            car = SUV()
        elif type == 2:
            car = Hybrid()
        elif type == 3 or type == 4:
            car = Bus()
        else:
            car = Truck()
        self.customer = car

    def change_method(self) -> None:
        curr_gas: str = self.method['gas']
        curr_amount: int = self.method['amount']
        print(
            f"Current Method: {curr_gas} / {curr_amount} Liters.")
        print()

        while True:
            print('0. Toggle fuel type')
            print('1. Change the amount of fuel')
            print('2. Finish')
            select = int(input('Select: '))
            print()

            if select == 0:
                self.method['gas'] = Name.GASOLINE if curr_gas == Name.DIESEL else Name.DIESEL
                curr_gas = self.method['gas']
                print(f"Fuel type changed: {curr_gas}")
                print()
            elif select == 1:
                while True:
                    amount = input(
                        "Enter 'F' (full), or the Amount of liters to fuel: ")
                    if amount == 'F':
                        self.method['amount'] = self.customer.capacity - \
                            self.customer.cur_fuel
                        print("Fueling method changed: Full\n")
                        curr_amount = self.customer.capacity - self.customer.cur_fuel
                    elif amount.isdigit():
                        amount = int(amount)
                        self.method['amount'] = amount
                        print(f"Fueling method changed: {amount}\n")
                        curr_amount = amount
                    else:
                        print("Wrong input!\n")
                        continue
                    break
            else:
                break

    def fuel(self) -> str:
        print("Checking the conditions...")
        curr_gas: str = self.method['gas']
        curr_amount: int = self.method['amount']

        fuel_type = self.customer.fuel_type
        if fuel_type != curr_gas:
            print(f"Requested: {fuel_type}, Selected: {curr_gas}\n")
            return 'wrong_fuel'

        curr_max = self.customer.capacity - self.customer.cur_fuel
        print(f'Fuel type: {fuel_type}')
        if self.customer.full == True and self.gas[curr_gas].remains < curr_max:
            print(
                f'Amount of {curr_gas} in the tank: {self.gas[curr_gas].remains} Liters, Tried: {curr_max}\n')
            return 'not_enough'
        if self.gas[curr_gas].remains < curr_amount:
            print(
                f'Amount of {curr_gas} in the tank: {self.gas[curr_gas].remains} Liters, Tried: {curr_amount}\n')
            return 'not_enough'

        if curr_max < curr_amount:
            print(
                f'Maximum amount to fuel: {curr_max} Liters, Tried: {curr_amount} Liters\n')
            return 'overflow'

        needs = self.customer.needed
        if needs != curr_amount:
            puts = curr_max if curr_amount == float('inf') else curr_amount
            print(f'Requested: {needs} Liters, Tried: {puts} Liters')
            return 'different'
        return 'good_job'

    def end(self) -> bool:
        if self.money < 5000:
            print('You should have at least $5000 to finish the game.')
            print(f'You have: ${self.money:.2f}\n')
            return False
        else:
            print('---------Summary---------')
            print(f'Rating: {self.rating}')
            print(f'Money: ${self.money:.2f}')
            print(f'Total customers handled: {self.total_num}')
            print('-----------------------------')
            print('You win!')
            return True


class Car:
    def __init__(self) -> None:
        self.fuel_type: str
        self.vehicle_type: str
        self.capacity: int
        self.cur_fuel: int
        self.needed: int
        self.full: bool
        self.cal_fuel()

    def cal_fuel(self) -> None:
        self.cur_fuel = int(self.capacity * random.uniform(0.1, 0.4))
        if random.randint(1, 4) == 1:
            self.full = True
            self.needed = self.capacity - self.cur_fuel
        else:
            self.full = False
            self.needed = int(((self.capacity - self.cur_fuel) *
                               random.uniform(0.5, 0.8) // 5) * 5)

    def printInfo(self) -> None:
        print("<<Vehicle Info>>")
        info = (
            f"Fuel type: {self.fuel_type}",
            f"Vehicle type: {self.vehicle_type}",
            f"Fuel: {self.cur_fuel} / {self.capacity}"
        )
        print(*info, sep=', ')


class Gasoline_Car(Car):
    def __init__(self) -> None:
        self.fuel_type = Name.GASOLINE
        super().__init__()


class Diesel_Car(Car):
    def __init__(self) -> None:
        self.fuel_type = Name.DIESEL
        super().__init__()

    def upgrade_claim(self) -> None:
        print("Driver: Refill the DEF, please.")
        print("Provide some DEF for free? (costs $50 yet increases rating by 3)")

    def upgrade(self) -> int:
        print("You provided some DEF for free")
        return 50


class SUV(Gasoline_Car):
    def __init__(self) -> None:
        self.vehicle_type = 'SUV'
        self.capacity = 80
        super().__init__()

    def upgrade_claim(self) -> None:
        print("Driver: An oil change, please.")
        print("Change the engine oil for free? (costs $100 yet increases rating by 3)")

    def upgrade(self) -> int:
        print("You replaced the engine oil for free")
        return 100


class Hybrid(Gasoline_Car):
    def __init__(self) -> None:
        self.vehicle_type = 'Hybrid'
        self.capacity = 60
        super().__init__()

    def upgrade_claim(self) -> None:
        print("Driver: My car has flat tires...")
        print("Change the tires for free? (costs $300 yet increases rating by 3)")

    def upgrade(self) -> int:
        print("You provided some tires for free")
        return 300


class Bus(Diesel_Car):
    def __init__(self) -> None:
        self.vehicle_type = 'Bus'
        self.capacity = 100
        super().__init__()


class Truck(Diesel_Car):
    def __init__(self) -> None:
        self.vehicle_type = 'Truck'
        self.capacity = 300
        super().__init__()


if __name__ == '__main__':
    main()
