/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "InputData.h"


/*
  CameraHandler handles all the game cameras

  Will be fleshed out later on when it becomes necessary to have multiple cameras
*/

class CameraHandler {
private:
  static Camera mainCam;
public:
  //key consts
  static const int FORWARD  = GLFW_KEY_W;
  static const int FORWARD2 = GLFW_KEY_UP;
  static const int BACK     = GLFW_KEY_S;
  static const int BACK2    = GLFW_KEY_DOWN;
  static const int RIGHT    = GLFW_KEY_D;
  static const int RIGHT2   = GLFW_KEY_RIGHT;
  static const int LEFT     = GLFW_KEY_A;
  static const int LEFT2    = GLFW_KEY_LEFT;
  static const int UP       = GLFW_KEY_SPACE;
  static const int DOWN     = GLFW_KEY_LEFT_SHIFT;
  static const int SPRINT   = GLFW_KEY_E;

  static void handleInput(InputData *data);

  static Camera* getCam();
};

#endif
