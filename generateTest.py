import random

with open("randomTest", "w") as test:
    y = int(input("Height? "))
    x = int(input("Length? "))

    test.write(str(y) + "\n")
    test.write(str(x) + "\n")

    current = 0
    for i in range(y):
        current += random.randint(0, 3)
        test.write(str(current if current <= x else x) + "\n")
