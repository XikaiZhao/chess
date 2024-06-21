SRC_DIR = ../..
BUILD_DIR = ../../../build
MCTS_LIBRARY=$(BUILD_DIR)/mcts/libMCTS.a

CXX = g++ -std=c++17
CXXFLAGS = -Wall -g -I$(SRC_DIR)

# Main application object files (without main.cpp)
SRC_SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/mcts/*.cpp)
SRC_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_SOURCES))
SRC_OBJECTS := $(filter-out $(BUILD_DIR)/main.o, $(SRC_OBJECTS))

# Test executable
TEST = $(BUILD_DIR)/tests/mctsTest/mctsTest

all: $(TEST)

$(TEST): $(BUILD_DIR)/tests/mctsTest/mctsTest.o $(SRC_OBJECTS) $(MCTS_LIBRARY)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L$(BUILD_DIR)/mcts -lMCTS

$(BUILD_DIR)/tests/mctsTest/mctsTest.o: mctsTest.cpp $(SRC_DIR)/Board.h $(SRC_DIR)/BoardWrapper.h $(SRC_DIR)/GameMCTSNodeState.h $(MCTS_LIBRARY)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c mctsTest.cpp -o $(BUILD_DIR)/tests/mctsTest/mctsTest.o -L$(BUILD_DIR)/mcts -lMCTS

clean:
	rm -f $(TEST) *.o
