/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include "../include/InputData.h"

bool InputData::getKey(int key) {
  bool res = keys[key];
  keys[key] = false;
  return res;
}

int InputData::getScrollY() {
  int res = scroll_y_offset;
  scroll_y_offset = 0;
  return res;
}

int InputData::getMouseX() {
  return mouse_x;
}

int InputData::getMouseY() {
  return mouse_y;
}

int InputData::getMouseXOffset() {
  return mouse_x - old_mouse_x;
}

int InputData::getMouseYOffset() {
  return old_mouse_y - mouse_y;
}
