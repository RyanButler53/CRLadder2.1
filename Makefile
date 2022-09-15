CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -pedantic #all debugging settings
CXX = clang++

TARGETS = main treequeue-test player-test

all:$(TARGETS)

main: main.o treequeue.o player.o cfgToSettings.o
	$(CXX) -o main main.o treequeue.o player.o cfgToSettings.o

treequeue-test: treequeue.o treequeue-test.o player.o
	$(CXX) -o treequeue-test treequeue-test.o treequeue.o player.o

player-test: player.o player-test.o
	$(CXX) -o player-test player-test.o player.o

main.o: main.cpp treequeue.hpp player.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

player.o: player.cpp player.hpp 
	$(CXX) $(CXXFLAGS) -c player.cpp

treequeue.o: treequeue.cpp treequeue.hpp player.hpp
	$(CXX) $(CXXFLAGS) -c treequeue.cpp

player-test.o: player-test.cpp player.cpp player.hpp
	$(CXX) $(CXXFLAGS) -c player-test.cpp

treequeue-test.o: treequeue-test.cpp treequeue.hpp player.hpp
	$(CXX) $(CXXFLAGS) -c treequeue-test.cpp

cfgToSettings.o: cfgToSettings.cpp cfgToSettings.hpp
	$(CXX) $(CXXFLAGS) -c cfgToSettings.cpp

clean:
	rm -rf *.o $(TARGETS)