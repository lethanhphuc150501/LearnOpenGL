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
    float input[] = {
    //  Position_A(0)   Position_B(1)
    //  0  1            2  3
        0, 0,           0, 0,
        0, 1,           0, -1,
        1, 0,           -1, 0
    };
    unsigned int vbo_id;
    unsigned int vao_id[2];
    glGenBuffers(1,       // số lượng buffer
                &vbo_id);  // ID của buffer [out]
    glGenVertexArrays(2,        // số lượng buffer
                    vao_id);    // ID của biến vao - dùng để lưu cấu hình nội dung các trường dữ liệu
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(input), input, GL_STATIC_DRAW);
        glBindVertexArray(vao_id[0]);   // VAO[0] vẽ tam giác A
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);   // Position_A
            glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        glBindVertexArray(vao_id[1]);   // VAO[1] vẽ tam giác B
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));   // Position_B
            glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // unbind, kết thúc làm việc với buffer
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao_id[0]);   // truyền vào VAO tương ứng với hình muốn vẽ, nếu không dùng VAO thì gọi lệnh vẽ sẽ lấy buffer được bind gần nhất
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(vao_id[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}
