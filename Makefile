# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
OBJS = main.o Player.o Board.o

# Default target
all: chess

# Target rules
chess: $(OBJS)
	$(CXX) $(CXXFLAGS) -o chess $(OBJS)

main.o: main.cpp Player.h Board.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Player.o: Player.h Player.cpp
	$(CXX) $(CXXFLAGS) -c Player.cpp

Board.o: Board.h Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp

# Clean
clean:
	rm -f *.o chess
