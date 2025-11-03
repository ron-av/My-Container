# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude

# Build and binary folders
BUILD_DIR := build
BIN_DIR   := bin

# Targets
MAIN_SRC  := Main.cpp
TEST_SRC  := tests.cpp
MAIN_BIN  := $(BIN_DIR)/Main
TEST_BIN  := $(BIN_DIR)/test


# Run the main demo program
Main: $(MAIN_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -o $(MAIN_BIN)
	@echo "Running Main..."
	@./$(MAIN_BIN)

# Run unit tests (test.cpp)
test: $(TEST_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_BIN)
	@echo "Running unit tests..."
	@./$(TEST_BIN)

# Run Valgrind to check for memory leaks (on the whole program)
valgrind: $(MAIN_SRC) $(TEST_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -o $(MAIN_BIN)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_BIN)
	@echo "Running Valgrind on Main..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(MAIN_BIN)
	@echo "Running Valgrind on Tests..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_BIN)

# Clean build and binary folders
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned build and binary files."

.PHONY: all Main test valgrind clean
