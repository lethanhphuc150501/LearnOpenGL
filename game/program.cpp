#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game.h"
#include "../shader.h"

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
    if (glfwInit() == false) return 0;
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return 0;

    Breakout.Init();

    float deltaTime = 0;
    float lastFrame = 0;

    
    while (!glfwWindowShouldClose(window)) {        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        Breakout.ProcessInput(deltaTime);
        Breakout.Update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();
        glfwSwapBuffers(window);
    }
}