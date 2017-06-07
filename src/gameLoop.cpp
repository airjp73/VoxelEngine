//GLFW
#include <GLFW/glfw3.h>

void runGameLoop(GLFWwindow *mainWindow) {
  GLfloat deltaTime = 0.0f;
  GLfloat lastFrame = 0.0f;
  while (!glfwWindowShouldClose(mainWindow)) {
    glfwPollEvents();
    glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(mainWindow);
  }
}
