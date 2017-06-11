/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//SOIL
#include <SOIL/SOIL.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/Camera.h"
#include "../include/callbacks.h"

bool keys[1024];
bool wireframe = false;
Camera camera;

void set_input_callbacks(GLFWwindow *mainWindow) {
  glfwSetKeyCallback(mainWindow, key_callback);
  glfwSetCursorPosCallback(mainWindow, mouse_callback);
  glfwSetScrollCallback(mainWindow, scroll_callback);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;

  if (keys[GLFW_KEY_ESCAPE])
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (keys[GLFW_KEY_F]) {
    if (!wireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      wireframe = true;
    }
    else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      wireframe = false;
    }
  }
}

bool firstMouse = true;
GLfloat lastX = 400, lastY = 300; //mouse pos
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  //calculate offsets
  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.rotate(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.zoom(yoffset);
}

void do_movement(GLfloat delta) {
  bool up     = keys[GLFW_KEY_UP] || keys[GLFW_KEY_W];
  bool down   = keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S];
  bool right  = keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D];
  bool left   = keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A];
  bool fly    = keys[GLFW_KEY_SPACE];
  bool fall   = keys[GLFW_KEY_LEFT_SHIFT];
  camera.move(up, down, right, left, fly, fall, delta);
}
