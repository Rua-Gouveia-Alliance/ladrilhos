
all:
	mkdir -p bin
	g++ -O3 -std=c++11 -Wall p1.cpp -o bin/p1 -lm

debug:
	g++ -O3 -std=c++11 -Wall -g p1.cpp -o bin/p1 -lm

clean:
	rm -r bin
