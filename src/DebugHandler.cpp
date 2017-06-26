/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

#include "../include/InputData.h"
#include "../include/DebugHandler.h"

bool DebugHandler::isWireframe = false;

void DebugHandler::handleInput(InputData *data, GLFWwindow *window) {
  if (data->getKey(GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (data->getKey(GLFW_KEY_F)) {
    if (isWireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      isWireframe = false;
    }
    else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      isWireframe = true;
    }
  }
}
