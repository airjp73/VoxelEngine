/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
#include "../include/InputData.h"

class DebugHandler {
private:
  static bool isWireframe;
public:
  static void handleInput(InputData *data, GLFWwindow *window);
};
