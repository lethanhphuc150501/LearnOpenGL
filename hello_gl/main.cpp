#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (glfwInit() == false) {
        return 0;
    }
    GLFWwindow* window = glfwCreateWindow(600, 600, "Hello OwO", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return 0;
    }

    unsigned int vertexShader;
    {
        const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "void main()\n"
            "{\n"
            " gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
            "}\0";
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    unsigned int fragmentShader;
    {
        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
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
    float input[] = {       // Dữ liệu thô
        0, 0,       // A0 - B0
        0, 1,       // A1
        0, -1,      // B1
        1, 0,       // A2
        -1, 0       // B2
    };
    unsigned int indice[] { // Dữ liệu cấp 2, lưu các index của dữ liệu thô
        0, 1, 3,    // Các index tương ứng với các đỉnh của tam giác A (A0, A1, A2)
        0, 2, 4     // Các index tương ứng với các đỉnh của tam giác B (B0, B1, B2)
    };
    unsigned int vbo_id;
    unsigned int vao_id;
    unsigned int ebo_id;
    glGenBuffers(1, &vbo_id);
    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(input), input, GL_STATIC_DRAW);
        glBindVertexArray(vao_id);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);  // dữ liệu về EBO phải được lưu trong VAO
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
            // Không unbind EBO khi VAO vẫn còn hoạt động
            // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // unbind, kết thúc làm việc với buffer
    glPolygonMode(GL_FRONT_AND_BACK,    // áp dụng cho cả 2 mặt trước và sau
                    GL_LINE);           // vẽ đường, không fill màu, != GL_FILL là fill màu 
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES,    // Hàm vẽ theo EBO
                        6,              // số index
                        GL_UNSIGNED_INT,    // kiểu data của index
                        0);             // don't care
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}
