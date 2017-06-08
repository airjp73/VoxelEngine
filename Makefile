INCLUDE = include
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))
OBJ = obj
SRC = src
BIN = bin
EXE = main
CC = g++

CPPFLAGS=$(shell pkg-config --cflags glfw3) -std=c++11
LDLIBS=$(shell pkg-config --static --libs glfw3) -lGLEW -lGL -lSOIL -I /usr/include/SOIL


all: $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o $(BIN)/$(EXE) $(LDLIBS)

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CC) -c $(CPPFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	$(CC) -c $(CPPFLAGS) $< -o $@ $(LDLIBS)


clean:
	-rm -f obj/*.o core
