# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I src

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/sim

# Find all .cpp files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Convert src/foo.cpp -> build/foo.o
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default rule
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile step
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean everything
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Optional: rebuild
re: clean all
