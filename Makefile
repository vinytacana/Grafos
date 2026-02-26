CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

all: main

main: Main.o Grafo.o
	$(CXX) $(CXXFLAGS) -o main Main.o Grafo.o

Main.o: Main.cpp Grafo.h
	$(CXX) $(CXXFLAGS) -c Main.cpp

Grafo.o: Grafo.cpp Grafo.h
	$(CXX) $(CXXFLAGS) -c Grafo.cpp

clean:
	rm -f *.o main
