def main():
    t = int(input())
    for i in range(t):
        N = int(input())
        X0Yplus_list = list()
        X0Yminus_list = list()
        Xplus_list = list()
        Xminus_list = list()
        for j in range(N):
            x, y = map(int, input().split(' '))
            if x == 0:
                if y > 0:
                    insert_sorted(X0Yplus_list, [x, y, y])
                if y < 0:
                    insert_sorted(X0Yminus_list, [x, y, y])
            elif x > 0:
                insert_sorted(Xplus_list, [x, y, y / x])
            else:
                insert_sorted(Xminus_list, [x, y, y / x])
        res_order = [X0Yplus_list, Xplus_list, X0Yminus_list, Xminus_list]
        for ls in res_order:
            ls: list[list[float, float, float]]
            for _x, _y, _ in ls:
                print(_x, _y)
        print("")


def insert_sorted(sorted_ls: list, val: list) -> None:
    if not sorted_ls:
        sorted_ls.append(val)
        return
    for i in range(len(sorted_ls)):
        if val[-1] > sorted_ls[i][-1]:
            sorted_ls.insert(i, val)
            return
        elif val[-1] == sorted_ls[i][-1]:
            if val[0] < sorted_ls[i][0]:
                sorted_ls.insert(i, val)
                return
            else:
                continue
        else:
            continue
    sorted_ls.append(val)


if __name__ == '__main__':
    main()
