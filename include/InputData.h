/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef INPUT_DATA_H
#define INPUT_DATA_H

class InputData {
private:
  bool keys[1024];
  bool mouseLeft, mouseRight;
  int mouse_x, mouse_y;
  int old_mouse_x, old_mouse_y;
  int scroll_x_offset, scroll_y_offset;

  friend class InputHandler;

public:
  bool getKey(int key);
  int getScrollY();
  int getMouseX();
  int getMouseY();
  int getMouseXOffset();
  int getMouseYOffset();
};

#endif
