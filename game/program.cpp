#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game.h"
#include "../shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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

    glfwSetKeyCallback(window, key_callback);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) Breakout.Keys[key] = true;
        else if (action == GLFW_RELEASE) Breakout.Keys[key] = false;
    }
}