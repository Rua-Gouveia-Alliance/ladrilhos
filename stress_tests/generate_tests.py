for s in range(5, 13):
    for i in range(s-1, s+1):
        with open(str(s)+"x"+str(i)+".in", "w") as test:
            test.write(str(s) + "\n")
            test.write(str(s) + "\n")

            for i in range(s):
                test.write(str(i) + "\n")
