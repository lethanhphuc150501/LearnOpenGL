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
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
        // Position(0) Color(1)    Radius(2)
        // 0  1         2  3  4     5
        0, 0,       // 0, 0, 0,    5
        0, 1, 
        1, 0
    };
    float input_1[] = {
        // Position(0) Color(1)    Radius(2)
        // 0  1         2  3  4     5
        0, 0,       // 0, 0, 0,    5
        0, -1, 
        -1, 0
    };
    unsigned int vbo_id[2];
    unsigned int vao_id[2];
    glGenBuffers(2,       // số lượng buffer
                vbo_id);  // ID của buffer [out]
    glGenVertexArrays(2,        // số lượng buffer
                    vao_id);    // ID của biến vao - dùng để lưu cấu hình nội dung các trường dữ liệu
    /* Khai báo ngữ cảnh: chọn VAO và buffer muốn làm việc */
    glBindVertexArray(vao_id[0]);
        glBindBuffer(GL_ARRAY_BUFFER,   // kiểu data là array
                     vbo_id[0]);        // buffer cần gán với data
            // copy data vào buffer (ram -> vram)
            glBufferData(GL_ARRAY_BUFFER, sizeof(input), input, GL_STATIC_DRAW);
            // glVertexAttribPointer(0,    // stt, Position = 0, Color = 1, Radius = 2 ...
            //                     2,      // số data trong trường, Position có 2 data (x,y), Color có 3 (r,g,b), Radius có 1
            //                     GL_FLOAT,   // kiểu dữ liệu của data
            //                     GL_FALSE,   // có chuẩn hóa hay không
            //                     6 * sizeof(float),  // bước nhảy đến trường Position của đỉnh tiếp theo = độ dài thông tin 1 đỉnh
            //                     (void*)0);  // vị trí bắt đầu data trong 1 hàng thông tin, Position bằng đầu từ ô thứ 0, Color từ ô thứ 2, Radius từ ô thứ 5
            // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)2);
            // glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)5);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);   // truyền vào index của thuộc tính Position = 0, Color = 1, Radius = 2
        glBindBuffer(GL_ARRAY_BUFFER, 0);   // unbind, kết thúc làm việc với buffer
    glBindVertexArray(0);               // unbind VAO

    glBindVertexArray(vao_id[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);   // bind 1 buffer khác để làm việc tiếp
            glBufferData(GL_ARRAY_BUFFER, sizeof(input_1), input_1, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao_id[0]);   // truyền vào VAO tương ứng với hình muốn vẽ, nếu không dùng VAO thì gọi lệnh vẽ sẽ lấy buffer được bind gần nhất
        glDrawArrays(GL_TRIANGLES,  // mode vẽ tam giác
                    0,              // bắt đầu vẽ từ đỉnh thứ 0
                    3);             // số đỉnh
        glBindVertexArray(vao_id[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    return 0;
}
