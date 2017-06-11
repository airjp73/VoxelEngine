/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#ifndef CAMERA_H
#define CAMERA_H

//GLEW
#include <GL/glew.h>
//GLM  -- OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
  //Vectors
  glm::vec3 worldUp;        //objective up of the worldspace
  glm::vec3 camPos;         //pos of cam itself
  glm::vec3 camFront;       //direction cam is pointing
  glm::vec3 camRight;       //points to cams positive x-axis
  glm::vec3 camUp;          //points to cams positive y-axis
  glm::mat4 view;

  //Euler Angles
  GLfloat yaw;
  GLfloat pitch;

  //Options
  GLfloat camSpeed;
  GLfloat mouseSensitivity;
  GLfloat fov;

public:
  //=========================
  //Constructor
  //=========================
  Camera();

  //=========================
  //Update vectors
  //=========================
  void updateVectors();

  //=========================
  //Getters
  //=========================
  glm::mat4 getViewMatrix();
  GLfloat getFov();

  //=========================
  //Move Camera
  //=========================

  /*  Moves the camera */
  void move(bool up, bool down, bool right, bool left, bool fly, bool fall, GLfloat deltaTime);

  /* Rotates the camera based on mouse movement */
  void rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

  /* Changes fov to zoom in based on mouse scroll */
  void zoom(GLfloat yoffset);
};

#endif
