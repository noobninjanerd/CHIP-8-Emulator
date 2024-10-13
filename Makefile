# -*- Makefile -*- 

# Define directories
SRC_DIR = ./src
OBJ_DIR = ./build/obj
BIN_DIR = ./build/bin
INCLUDE_DIR = ./include

# Compilers (predefined variables, so no need to define)
# CC = gcc	# C compiler
# CXX = g++	# C++ compiler

# Define the source files, object files and target executable
TARGET = $(BIN_DIR)/chip8_emul
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# preprocessor flags
CPPFLAGS = -I$(INCLUDE_DIR)

# compilation flags
CXXFLAGS = -Wall -g

# linking flags
LDFLAGS = 
LDLIBS 	= -lSDL2

.PHONY: all clean

# default rule to build the target
all: $(TARGET)

# rule to build the target from object files: Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# rule to build object files from source files: Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS)

# @: name of the target being generated
# <: first dependency of the specific rule
# ^: all dependencies of the rule
# ?: dependencies newer than the target

## $(patsubst pattern, replacement, text)
# pattern: what you are searching for
# replacement: what you want to replace the matched part with
# text: the string/list of files you want to apply the pattern to
