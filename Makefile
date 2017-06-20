
## VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
## © Aaron Pettengill 2017

#Compiler and c++ flags
CC = g++
WIN_CC = x86_64-w64-mingw32-g++
CPPFLAGS= -std=c++11

#Project Directories
OBJ = obj
SRC = src
BIN = bin
WIN = win
LIB = libs
INCLUDE = include

#Executable
EXE = main
WIN_EXE = main.exe

#Source files and object files
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))
WINOBJ  = $(patsubst $(SRC)/%.cpp, $(WIN)/%.o, $(SOURCES))

#Libraries included in lib folder
LIBS     = $(FASTNOISE_OBJ)
WIN_LIBS = $(FASTNOISE_WIN)

FASTNOISE_DIR = FastNoise
FASTNOISE_SRC = $(LIB)/$(FASTNOISE_DIR)/FastNoise.cpp
FASTNOISE_INC = $(LIB)/$(FASTNOISE_DIR)/FastNoise.h
FASTNOISE_OBJ = $(LIB)/$(OBJ)/FastNoise.o
FASTNOISE_WIN = $(WIN)/FastNoise.o

#Library Flags
LDLIBS = $(GLFW) $(GLEW) $(OPENGL) $(SOIL)

GLFW   = $(shell pkg-config --cflags glfw3) $(shell pkg-config --static --libs glfw3)
GLEW   = -lGLEW
OPENGL = -lGL
SOIL   = -lSOIL -I /usr/include/SOIL


#### COMPILE RULES
all: $(LIBS) $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o $(BIN)/$(EXE) $(LDLIBS) -g -O0

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CC) -c $(CPPFLAGS) $^ -o $@ -g

$(OBJ)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	$(CC) -c $(CPPFLAGS) $< -o $@ -g

$(FASTNOISE_OBJ): $(FASTNOISE_SRC) $(FASTNOISE_INC)
	$(CC) -c $(CPPFLAGS) $< -o $@

#### Windows
WIN_MAGIC = -I/usr/i486-mingw32/include -L/usr/i486-mingw32/lib -mwindows -lglew32  -static-libgcc
windows: $(WIN_LIBS) $(WINOBJ)
	$(WIN_CC) $(CPPFLAGS) $^ -o $(WIN)/$(WIN_EXE) $(WIN_MAGIC)

$(WIN)/main.o: $(SRC)/main.cpp
	$(WIN_CC) -c $(CPPFLAGS) $^ -o $@ $(WIN_MAGIC) $(LDLIBS)

$(WIN)/%.o: $(SRC)/%.cpp $(INCLUDE)/%.h
	$(WIN_CC) -c $(CPPFLAGS) $^ -o $@ $(WIN_MAGIC) $(LDLIBS)

$(FASTNOISE_WIN): $(FASTNOISE_SRC) $(FASTNOISE_INC)
	$(WIN_CC) -c $(CPPFLAGS) $< -o $@ $(WIN_MAGIC) $(LDLIBS)

#### CLEAN
clean:
	-rm -f $(OBJ)/*.o
	-rm -f $(LIB)/$(OBJ)/*.o
	-rm -f $(BIN)/$(EXE)
	-rm -f $(WIN)/*.o
	-rm -f $(LIB)/$(WIN)/*.o
	-rm -f $(WIN)/$(WIN_EXE)
