#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (glfwInit() == false) return 0;
    GLFWwindow* window = glfwCreateWindow(800, 800, "Exercise 2", nullptr, nullptr);
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
    unsigned int fragmentShader_orange;
    {
        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";
        fragmentShader_orange = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader_orange, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader_orange);
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader_orange, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragmentShader_orange, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    unsigned int fragmentShader_yellow;
    {
        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
            "}\0";
        fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader_yellow);
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    unsigned int shaderProgram_orange;
    shaderProgram_orange = glCreateProgram();
    glAttachShader(shaderProgram_orange, vertexShader);
    glAttachShader(shaderProgram_orange, fragmentShader_orange);
    glLinkProgram(shaderProgram_orange);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram_orange, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram_orange, 512, NULL, infoLog);
    }

    unsigned int shaderProgram_yellow;
    shaderProgram_yellow = glCreateProgram();
    glAttachShader(shaderProgram_yellow, vertexShader);
    glAttachShader(shaderProgram_yellow, fragmentShader_yellow);
    glLinkProgram(shaderProgram_yellow);
    glGetProgramiv(shaderProgram_yellow, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram_yellow, 512, NULL, infoLog);
    }

    float input_0[] = {
        -1, 0,
        0, 0,
        0, 1
    };
    float input_1[] = {
        0, 0,
        1, 0,
        0, 1
    };
    unsigned int vbo[2];
    unsigned int vao[2];
    glGenBuffers(2, vbo);
    glGenVertexArrays(2, vao);
    glBindVertexArray(vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(input_0), input_0, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindVertexArray(vao[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(input_1), input_1, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram_orange);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgram_yellow);
        glBindVertexArray(vao[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}