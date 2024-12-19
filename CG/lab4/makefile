# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./glad

# Directories
SRC_DIR = .
GLAD_DIR = ./glad
BUILD_DIR = ./build

# Source files
SRC = $(SRC_DIR)/main.cpp $(GLAD_DIR)/glad.c
OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/glad.o

# Output
TARGET = phong_lighting

# Libraries
LIBS = -lGL -lsfml-graphics -lsfml-window -lsfml-system

# Rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

$(BUILD_DIR)/glad.o: $(GLAD_DIR)/glad.c
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
