# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
all: clean compile link

clean:
	rm -f bin/*.o

compile:
	g++ -g -Wall -Weffc++ -std+c++11 -c -Include -o bin/main.o src/main.cpp

link:
	g++ -o bin/simulation bin/main.o

run:
	./simulation