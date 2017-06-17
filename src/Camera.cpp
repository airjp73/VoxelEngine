/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//GLM  -- OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//header
#include "../include/Camera.h"

const GLfloat YAW         = -90.0f;
const GLfloat PITCH       = 0.0f;
const GLfloat SPEED       = 5.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat FOV         = 45.0f;

Camera::Camera() {
  //Init to default values (pointing at origin)
  worldUp   = glm::vec3(0.0f, 1.0f, 0.0f);
  camPos    = glm::vec3(0.0f, 0.0f, 3.0f);
  yaw       = YAW;
  pitch     = PITCH;

  camSpeed          = 10.0f;
  mouseSensitivity  = .25f;
  fov               = 45.0f;

  updateVectors();
}

//=========================
//Update vectors
//=========================
void Camera::updateVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  camFront = glm::normalize(front);
  front.y = 0;
  camForward = glm::normalize(front);
  camRight = glm::normalize(glm::cross(front, worldUp));
  camUp =    glm::normalize(glm::cross(camRight, camFront));
}

//=========================
//Getters
//=========================
glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(camPos, camPos + camFront, camUp);
}

GLfloat Camera::getFov() {
  return fov;
}

//=========================
//Move Camera
//=========================

/*  Moves the camera */
void Camera::move(bool up, bool down, bool right, bool left, bool fly, bool fall, GLfloat deltaTime) {
  GLfloat velocity = camSpeed * deltaTime;
  if (isSprinting)
    velocity *= 10;
  if (up)
    camPos += camForward * velocity;
  if (down)
    camPos -= camForward * velocity;
  if (right)
    camPos += camRight * velocity;
  if (left)
    camPos -= camRight * velocity;
  if (fly)
    camPos += worldUp * velocity;
  if (fall)
    camPos -= worldUp * velocity;
}

/* Rotates the camera based on mouse movement */
void Camera::rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (constrainPitch) {
    if (pitch > 89.0f)
      pitch = 89.0f;
    else if (pitch < -89.0f)
      pitch = -89.0f;
  }

  updateVectors();
}

/* Changes fov to zoom in based on mouse scroll */
void Camera::zoom(GLfloat yoffset) {
  fov -= yoffset;
  if (fov < 1.0f)
    fov = 1.0f;
  else if (fov > 45.0f)
    fov = 45.0f;
}

void Camera::setSprint(bool sprinting) {
  isSprinting = sprinting;
}
