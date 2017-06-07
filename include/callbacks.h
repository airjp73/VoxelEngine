#ifndef CALLBACKS_H
#define CALLBACKS_H
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

//VoxelEngine
#include "Camera.h"

extern bool keys[1024];
extern Camera camera;

void set_input_callbacks(GLFWwindow *mainWindow);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void do_movement(GLfloat delta);

#endif
