#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (glfwInit() == false) return 0;
    GLFWwindow* window = glfwCreateWindow(800, 800, "Exercise 1", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return 0;
    unsigned int vertexShader;
    {
        const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "void main() {\n"
            "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
            "}\0";
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    unsigned int fragmentShader;
    {
        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glUseProgram(shaderProgram);

    float inputs[] = {
        -1, 0,
        0, 0,
        0, 1,
        0, 0,
        1, 0,
        0, 1
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(inputs), inputs, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}