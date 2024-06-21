SRC_DIR = src
MCTS_DIR = src/mcts
NNUE_DIR = src/nnue
TEST_DIRS := $(wildcard src/tests/*)
BUILD_DIR = bin

# Variables
CXX = g++ -pg
CXXFLAGS = -Wall -std=c++17 -Ofast -ffast-math
AR=ar
ARFLAGS=rcs

# Targets
TARGET = $(BUILD_DIR)/chess
MCTS_LIBRARY=$(BUILD_DIR)/mcts/libMCTS.a
NNUE_LIBRARY=$(BUILD_DIR)/nnue/libNNUE.a

# Source files
SRC_SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/mcts/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_SOURCES))
OBJECTS := $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS))

# MCTS files
MCTS_OBJECTS := $(patsubst $(MCTS_DIR)/%.cpp, $(BUILD_DIR)/mcts/%.o, $(wildcard $(MCTS_DIR)/*.cpp))

# NNUE files
NNUE_OBJECTS := $(patsubst $(NNUE_DIR)/%.cpp, $(BUILD_DIR)/nnue/%.o, $(wildcard $(NNUE_DIR)/*.cpp))

# include directory
INCLUDES=-I $(SRC_DIR)/mcts -I $(SRC_DIR)/nnue

all: $(TARGET) $(TEST_DIRS)

# Target rules
$(TARGET): $(OBJECTS) $(BUILD_DIR)/main.o $(MCTS_LIBRARY) $(NNUE_LIBRARY) 
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(BUILD_DIR)/mcts -lMCTS -L$(BUILD_DIR)/nnue -lNNUE

# Rule to make MCTS library
$(BUILD_DIR)/mcts/MCTS.o: $(SRC_DIR)/mcts/MCTS.cpp $(SRC_DIR)/mcts/MCTS.cpp $(SRC_DIR)/mcts/MCTS.h $(SRC_DIR)/mcts/MCTSNodeState.h $(SRC_DIR)/mcts/Timer.h
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/mcts/MCTS.cpp -o $(BUILD_DIR)/mcts/MCTS.o
$(MCTS_LIBRARY): $(MCTS_OBJECTS)
	$(AR) $(ARFLAGS) $@ $(MCTS_OBJECTS)

# Rule to make NNUE library
$(BUILD_DIR)/nnue/NNUE.o: $(SRC_DIR)/nnue/NNUE.cpp $(SRC_DIR)/nnue/NNUE.h $(SRC_DIR)/nnue/NNUEModel.h
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/nnue/NNUE.cpp -o $(BUILD_DIR)/nnue/NNUE.o
$(NNUE_LIBRARY): $(NNUE_OBJECTS)
	$(AR) $(ARFLAGS) $@ $(NNUE_OBJECTS)

# Object files
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/Player.h $(SRC_DIR)/Board.h $(SRC_DIR)/nnue/NNUEModel.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/Player.o: $(SRC_DIR)/Player.cpp  $(SRC_DIR)/Player.h $(SRC_DIR)/Player.cpp $(SRC_DIR)/Board.h $(SRC_DIR)/Defs.h $(SRC_DIR)/Piece.h $(SRC_DIR)/King.h $(SRC_DIR)/BoardWrapper.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Player.cpp -o $(BUILD_DIR)/Player.o

$(BUILD_DIR)/Piece.o: $(SRC_DIR)/Piece.cpp $(SRC_DIR)/Piece.h $(SRC_DIR)/Piece.cpp $(SRC_DIR)/Board.h $(SRC_DIR)/Defs.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Piece.cpp -o $(BUILD_DIR)/Piece.o

$(BUILD_DIR)/King.o: $(SRC_DIR)/King.cpp $(SRC_DIR)/King.h $(SRC_DIR)/King.cpp $(SRC_DIR)/Board.h $(SRC_DIR)/Defs.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/King.cpp -o $(BUILD_DIR)/King.o

$(BUILD_DIR)/Board.o: $(SRC_DIR)/Board.cpp $(SRC_DIR)/Board.h $(SRC_DIR)/Board.cpp $(SRC_DIR)/Defs.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/Board.cpp -o $(BUILD_DIR)/Board.o

$(BUILD_DIR)/GameMCTSNodeState.o: $(SRC_DIR)/GameMCTSNodeState.cpp $(SRC_DIR)/GameMCTSNodeState.h $(SRC_DIR)/Board.h $(SRC_DIR)/Player.h $(SRC_DIR)/Piece.h $(SRC_DIR)/King.h $(MCTS_LIBRARY) $(NNUE_LIBRARY)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/GameMCTSNodeState.cpp -o $(BUILD_DIR)/GameMCTSNodeState.o -L$(BUILD_DIR)/mcts -lMCTS -L$(BUILD_DIR)/nnue -lNNUE

$(BUILD_DIR)/MCTS_instantiate.o: $(SRC_DIR)/MCTS_instantiate.cpp 
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/MCTS_instantiate.cpp -o $(BUILD_DIR)/MCTS_instantiate.o

# Rule to make each test
$(TEST_DIRS):
	$(MAKE) -C $@

# Clean
clean:
	rm -f $(BUILD_DIR)/main.o $(OBJECTS) $(TARGET) $(BUILD_DIR)/tests/legalMoveTest.o $(MCTS_LIBRARY) $(MCTS_OBJECTS) $(NNUE_LIBRARY) $(NNUE_OBJECTS)
	for dir in $(TEST_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: all $(TEST_DIRS) clean