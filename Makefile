# Use g++
CXX = g++
CXXFLAGS = -fPIC \
           -Iinclude \
           -Wall \
           -Wextra \
           -std=c++17 \
           -fpermissive \
           -pthread
LDFLAGS = -shared

# Directories path
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
LIB_DIR = lib

# Output library and executable
LIB_NAME = libutils.so
LIB_OUTPUT = $(LIB_DIR)/$(LIB_NAME)
EXECUTABLE = main

# Source files and object files
SOURCES = $(SRC_DIR)/Timer.cpp \
          $(SRC_DIR)/Handler.cpp \
          $(SRC_DIR)/Looper.cpp \
          $(SRC_DIR)/MessageQueue.cpp

# Object files corresponding to the source files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target: build library and executable
all: $(LIB_OUTPUT) $(EXECUTABLE)

# Rule to create the library
$(LIB_OUTPUT): $(OBJECTS) | $(LIB_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Rule to build the executable
$(EXECUTABLE): main.cpp $(LIB_OUTPUT)
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -L$(LIB_DIR) -lutils

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(EXECUTABLE)

# Run with the correct library path
run:
	LD_LIBRARY_PATH=./lib ./main

.PHONY: all clean run
