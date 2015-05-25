all: main

main: main.o grid.o
	g++ -o main.out main.o grid.o

main.o: main.cpp
	g++ -c main.cpp

grid.o: grid.cpp
	g++ -c grid.cpp

clean:
	rm -rf *o main.out

run:
	g++ -o main.out main.cpp grid.cpp
	./main.out