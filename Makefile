all: main

main: main.o grid.o
	g++ -o main main.o grid.o

main.o: main.cpp
	g++ -c main.cpp

grid.o: grid.cpp
	g++ -c grid.cpp

clean:
	rm -rf *o main

run:
	g++ -o main main.cpp grid.cpp
	./main