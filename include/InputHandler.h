/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//VoxelEngine
#include "InputData.h"

/*
  InputHandler takes raw input data, packages it up and sends it to other higher level input handlers
  The higher level input handlers then take the data, and use it
*/

class InputHandler {
private:
  static InputData *curData;
  static GLFWwindow *mainWindow;

public:
  //sets callback functions with glfw
  static void init(GLFWwindow *window);
  //glfw callbacks
  static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
  static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

  //poll events
  static void poll();
  //process input data
  static void process_input();
};

#endif
