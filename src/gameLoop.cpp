/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "../include/callbacks.h"
#include "../include/Camera.h"
#include "../include/World.h"

void runGameLoop(GLFWwindow *mainWindow, float windowWidth, float windowHeight) {
  World gameWorld(glm::ivec3(0,0,0));

  glm::mat4 projection = glm::perspective(camera.getFov(), windowWidth/windowHeight, 0.1f, 100.0f);
  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;

  while (!glfwWindowShouldClose(mainWindow)) {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glfwPollEvents();
    do_movement(deltaTime);

    glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 view;
    view = camera.getViewMatrix();

    gameWorld.render(view, projection);

    glfwSwapBuffers(mainWindow);
  }
}
