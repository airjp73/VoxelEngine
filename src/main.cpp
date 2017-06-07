//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//c++ std
#include <iostream>

//VoxelEngine
#include "../include/gameLoop.h"
#include "../include/callbacks.h"

//const vars
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;


int main() {
  //============================
  //Initial setup
  //============================
  //Init glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  //Create GLFWwindow
  GLFWwindow *mainWindow = glfwCreateWindow(
    WINDOW_WIDTH, WINDOW_HEIGHT,
    "VoxelEngine", nullptr, nullptr
  );
  glfwMakeContextCurrent(mainWindow);
  glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  //Init GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Error -- Failed to init GLEW\n";
    return 1;
  }

  //set viewport
  int width, height;
  glfwGetFramebufferSize(mainWindow, &width, &height);
  glViewport(0, 0, width, height);
  float screenWidth = width;
  float screenHeight = height;

  //Input Callbacks (callbacks.cpp)
  set_input_callbacks(mainWindow);

  //OpenGL settings
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);


  //============================
  //Run Game Loop
  //============================
  runGameLoop(mainWindow);

  //============================
  //Exit Cleanup
  //============================
  glfwTerminate();
  return 0;
}
