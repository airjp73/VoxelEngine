
## VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
## © Aaron Pettengill 2017

#Compiler and c++ flags
CC = g++
CPPFLAGS= -std=c++11

#Project Directories
OBJ = obj
SRC = src
BIN = bin
LIB = libs
INCLUDE = include

#Executable
EXE = main

#Source files and object files
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

#Libraries included in lib folder
LIBS = $(FASTNOISE_OBJ)

FASTNOISE_DIR = FastNoise
FASTNOISE_SRC = $(LIB)/$(FASTNOISE_DIR)/FastNoise.cpp
FASTNOISE_INC = $(LIB)/$(FASTNOISE_DIR)/FastNoise.h
FASTNOISE_OBJ = $(LIB)/$(OBJ)/FastNoise.o

#Library Flags
LDLIBS = $(GLFW) $(GLEW) $(OPENGL) $(SOIL)

GLFW   = $(shell pkg-config --cflags glfw3) $(shell pkg-config --static --libs glfw3)
GLEW   = -lGLEW
OPENGL = -lGL
SOIL   = -lSOIL -I /usr/include/SOIL


#### COMPILE RULES
all: $(LIBS) $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o $(BIN)/$(EXE) $(LDLIBS)

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CC) -c $(CPPFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	$(CC) -c $(CPPFLAGS) $< -o $@

$(FASTNOISE_OBJ): $(FASTNOISE_SRC) $(FASTNOISE_INC)
	$(CC) -c $(CPPFLAGS) $< -o $@

#### CLEAN
clean:
	-rm -f $(OBJ)/*.o
	-rm -f $(LIB)/$(OBJ)/*.o
	-rm -f $(BIN)/$(EXE)
