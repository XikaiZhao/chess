# Variables
CXX = g++ -g
CXXFLAGS = -Wall -std=c++17
OBJS = legalMoveTests.o Player.o Board.o Piece.o

# Default target
all: legalMoveTest

# Target rules
legalMoveTest: $(OBJS)
	$(CXX) $(CXXFLAGS) -o legalMoveTest $(OBJS)

main.o: legalMoveTests.cpp ../Player.h ../Board.h
	$(CXX) $(CXXFLAGS) -c legalMoveTests.cpp

Player.o: ../Player.h ../Player.cpp ../Defs.h
	$(CXX) $(CXXFLAGS) -c ../Player.cpp

Board.o: ../Board.h ../Board.cpp ../Piece.h ../Defs.h
	$(CXX) $(CXXFLAGS) -c ../Board.cpp

Piece.o: ../Piece.h ../Piece.cpp ../Board.h ../Defs.h
	$(CXX) $(CXXFLAGS) -c ../Piece.cpp

# Clean
clean:
	rm -f *.o legalMoveTest
