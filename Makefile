all: main

main: main.o grid.o
	g++ main.o grid.o

main.o: main.cpp
	g++ -c main.cpp

grid.o: grid.cpp
	g++ -c grid.cpp

clean:
	rm -rf *o a.out

run:
	g++ main.cpp grid.cpp
	./a.out