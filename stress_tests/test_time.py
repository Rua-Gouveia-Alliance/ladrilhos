import os

os.system("echo > results.txt")
os.system("echo > count.txt")
for s in range(5, 13):
    for i in range(s-1, s+1):
        file = str(s)+"x"+str(i)
        os.system("echo " + file + ">> results.txt")
        os.system("echo " + file + ">> count.txt")
        file += ".in"
        os.system("{ time ./../bin/p1 < " + file + " >> count.txt ; } 2>> results.txt")
        os.system("echo >> results.txt")
        os.system("echo >> count.txt")

