# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: clean compile link

clean:
	rm -f bin/*

link:
	@echo "Linking Object files"
	g++ -o  bin/simulation bin/settlement.o bin/main.o bin/Auxiliary.o bin/Facility.o bin/SelectionPolicy.o bin/plan.o bin/Simulation.o bin/Action.o

compile: 
	@echo "Compiling source code"
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/main.o src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/Facility.o src/Facility.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/settlement.o src/settlement.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/plan.o src/plan.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/Simulation.o src/Simulation.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -I./include -o bin/Action.o src/Action.cpp