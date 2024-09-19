#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "../shader.h"

int main() {
    if (glfwInit() == false) return 0;
    GLFWwindow* window = glfwCreateWindow(800, 800, "Shaders", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return 0;
    
    Shader ourShader("shader.vs", "shader.fs");
    ourShader.use();
    float inputs[] = {
        // Position(0)  // Color
        -1, 0,          1, 1, 1,
        0, 0,           0, 1, 1,
        0, 1,           1, 0, 1
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(inputs), inputs, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}