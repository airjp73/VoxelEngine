/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

#include "../include/InputHandler.h"
#include "../include/CameraHandler.h"
#include "../include/InputData.h"
#include "../include/DebugHandler.h"

GLFWwindow *InputHandler::mainWindow = nullptr;
InputData *InputHandler::curData = nullptr;

void InputHandler::init(GLFWwindow *window) {
  mainWindow = window;
  curData = new InputData();
  glfwSetKeyCallback(mainWindow, key_callback);
  glfwSetCursorPosCallback(mainWindow, cursor_pos_callback);
  glfwSetScrollCallback(mainWindow, scroll_callback);
}

void InputHandler::cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
  curData->mouse_x = xpos;
  curData->mouse_y = ypos;
}

void InputHandler::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  if (action == GLFW_PRESS)
    curData->keys[key] = true;
  else if (action == GLFW_RELEASE)
    curData->keys[key] = false;
}

void InputHandler::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  curData->scroll_x_offset += xoffset;
  curData->scroll_y_offset += yoffset;
}

void InputHandler::poll() {
  glfwPollEvents();
}

void InputHandler::process_input() {
  InputData *nextFrame = new InputData(*curData);
  nextFrame->scroll_x_offset = 0;
  nextFrame->scroll_y_offset = 0;
  nextFrame->old_mouse_x = nextFrame->mouse_x;
  nextFrame->old_mouse_y = nextFrame->mouse_y;


    //input prioritized in this order
      //debug
      //menus
      //camera
      //player

  DebugHandler::handleInput(curData, mainWindow);
  //checkMenus();
  CameraHandler::handleInput(curData);
  //checkPlayer():
  curData = nextFrame;
}
