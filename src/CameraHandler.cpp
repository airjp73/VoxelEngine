/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include "../include/Camera.h"
#include "../include/InputData.h"
#include "../include/CameraHandler.h"

Camera CameraHandler::mainCam;

void CameraHandler::handleInput(InputData *data) {
  bool forward  = data->getKey(FORWARD)  || data->getKey(FORWARD2);
  bool back     = data->getKey(BACK)     || data->getKey(BACK2);
  bool right    = data->getKey(RIGHT)    || data->getKey(RIGHT2);
  bool left     = data->getKey(LEFT)     || data->getKey(LEFT2);
  bool up       = data->getKey(UP);
  bool down     = data->getKey(DOWN);

  mainCam.setSprint(data->getKey(SPRINT));
  mainCam.move(forward, back, right, left, up, down);
  mainCam.zoom(data->getScrollY());
  mainCam.rotate(data->getMouseXOffset(), data->getMouseYOffset());
}

Camera* CameraHandler::getCam() {
  return &mainCam;
}
