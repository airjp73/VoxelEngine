INCLUDE = include
SRC = src
OBJ = obj
BIN = bin
EXE = main
CC = g++

CPPFLAGS=$(shell pkg-config --cflags glfw3) -std=c++11
LDLIBS=$(shell pkg-config --static --libs glfw3) -lGLEW -lGL -lSOIL -I /usr/include/SOIL

OBJ_FILES = $(OBJ)/callbacks.o $(OBJ)/Camera.o $(OBJ)/gameLoop.o

all: $(SRC)/main.cpp $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $^ -o $(BIN)/$(EXE) $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	$(CC) -c $(CPPFLAGS) $< -o $@ $(LDLIBS)


clean:
	-rm -f obj/*.o core
