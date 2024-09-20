#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../shader.h"

int main() {
    if (glfwInit() == false) return 0;
    GLFWwindow* window = glfwCreateWindow(800, 800, "Exercise 2", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return 0;
    
    Shader exercise2 = Shader("HorizontalOffset.vs", "orange.fs");
    exercise2.use();

    float inputs[] = {
        -1, 0,
        0, 0,
        0, 1
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(inputs), inputs, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float offset = sin(timeValue);
        int vertexColorLocation = glGetUniformLocation(exercise2.ID, "x_off");        
        glUniform1f(vertexColorLocation, offset);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}