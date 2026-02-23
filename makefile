# Program Name
PROGRAM_NAME = chip8

# Define directories
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Define the compiler and flags
CC := g++
CFLAGS := -Wall -Wextra -g 
INCLUDE := -I "C:\Libraries\SDL3-devel-3.4.2-mingw\SDL3-3.4.2\x86_64-w64-mingw32\include" -L "C:\Libraries\SDL3-devel-3.4.2-mingw\SDL3-3.4.2\x86_64-w64-mingw32\lib" -lSDL3

# Find all source files in the source directory
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Generate the list of object file names with the OBJ_DIR prefix
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Define the executable name
EXECUTABLE := $(BIN_DIR)/$(PROGRAM_NAME)

# The default target
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDE)

# Pattern rule to build object files in the build directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

# Target to create the build and bin directories if they don't exist
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean

print:
	echo SRC_DIR=${SRC_DIR}
	echo OBJ_DIR=${OBJ_DIR}