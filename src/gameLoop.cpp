/*
VoxelEngine is licensed under https://creativecommons.org/licenses/by-nc/4.0/
© Aaron Pettengill 2017
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

//#include "../include/callbacks.h"
#include "../include/Camera.h"
#include "../include/World.h"
#include "../include/Logger.h"
#include "../include/TaskScheduler.h"
#include "../include/ITask.h"
#include "../include/CameraHandler.h"
#include "../include/InputHandler.h"

const float FRAME_TIME = 0.016f;

void runGameLoop(GLFWwindow *mainWindow, float windowWidth, float windowHeight) {
  Logger gameLoopLog("Game Loop");
  World gameWorld(glm::ivec3(0,0,0));

  glm::mat4 projection = glm::perspective(CameraHandler::getCam()->getFov(), windowWidth/windowHeight, 0.1f, 1000.0f);

  //time vars for keeping fixed timestep
  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;
  GLfloat currentFrame = 0.0f;

  //time vars for counting render fps
  GLfloat fpsTime = 0.0f; //don't display fps every frame
  GLfloat fpsDelta = 0.0f;
  int fpsCount = 0;

  while (!glfwWindowShouldClose(mainWindow)) {

    //if game is running faster than fixed time step, keep polling for events
    while (deltaTime < FRAME_TIME) {
      currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      InputHandler::poll();
    }
    fpsDelta = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //if game is running slower than fixed time step catch up the game logic before rendering
    while (deltaTime > FRAME_TIME) {
      deltaTime -= FRAME_TIME;

      /////Input
      InputHandler::poll();
      InputHandler::process_input();


      /////Game stuff

    }

    /////Keep track of render fps
    fpsTime += fpsDelta;
    ++fpsCount;
    if (fpsTime >= 1.0f) {
      std::string fpsMsg = std::to_string(fpsCount) + " fps";
      gameLoopLog.log(fpsMsg, Logger::DEBUG);
      fpsTime = 0.0f;
      fpsCount = 0;
    }

    /////Render
    glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view;
    view = CameraHandler::getCam()->getViewMatrix();
    gameWorld.render(view, projection);

    glfwSwapBuffers(mainWindow);
  }
}
