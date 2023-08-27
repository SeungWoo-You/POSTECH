def main():
    t = int(input())
    for _ in range(t):
        n = int(input())
        courses = []
        for _ in range(n):
            s, e = map(int, input().split())
            courses.append((s, e))
        print(count_max_schedule(courses))


def count_max_schedule(courses: list) -> int:
    end = -1
    ordered = sorted(courses, key=lambda x: x[1])
    count = 0
    for s, e in ordered:
        if end <= s:
            end = e
            count += 1
    return count


if __name__ == '__main__':
    main()
