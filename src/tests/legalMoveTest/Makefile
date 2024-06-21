SRC_DIR = ../..
BUILD_DIR = ../../../bin

CXX = g++ -std=c++17
CXXFLAGS = -Wall -g -I$(SRC_DIR) -I$(SRC_DIR)/mcts -I$(SRC_DIR)/nnue

# Main application object files (without main.cpp)
SRC_SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/mcts/*.cpp) $(wildcard $(SRC_DIR)/nnue/*.cpp)
SRC_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_SOURCES))
SRC_OBJECTS := $(filter-out $(BUILD_DIR)/main.o, $(SRC_OBJECTS))

# Test executable
TEST = $(BUILD_DIR)/tests/legalMoveTest/legalMoveTest

all: $(TEST)

$(TEST): $(BUILD_DIR)/tests/legalMoveTest/legalMoveTest.o $(SRC_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/tests/legalMoveTest/legalMoveTest.o: legalMoveTest.cpp SimpleJSONReader.h $(SRC_DIR)/Board.h $(SRC_DIR)/BoardWrapper.h
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c legalMoveTest.cpp -o $(BUILD_DIR)/tests/legalMoveTest/legalMoveTest.o

clean:
	rm -f $(TEST) *.o
