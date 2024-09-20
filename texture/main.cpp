#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

int main() {
    if (glfwInit() == false) return 0;
    GLFWwindow* window = glfwCreateWindow(800, 800, "Texture", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return 0;
    
    Shader txtr = Shader("main.vs", "main.fs");
    txtr.use();

    float inputs[] = {
        // positions    // texture coords   // color
        -1, 0,          0, 1,               0, 0, 1,
        0, 0,           1, 1,               1, 0, 0,
        0, 1,           1, 0,               0, 1, 0
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(inputs), inputs, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("cat.jpg",  // Kích thước ảnh phải chia hết cho 512 để không bị padding, chỉ dùng được ảnh .jpg
                                    &width, &height, &nrChannels, 0);    // load data
    if (data) {
        glTexImage2D(GL_TEXTURE_2D,
                    0,              // basic level, không zoom to nhỏ
                    GL_RGB,         // RGB format
                    width, height,
                    0,              // luôn bằng 0
                    GL_RGB, GL_UNSIGNED_BYTE,   // RGB format, input data là unsigned char
                    data);          // data
        glGenerateMipmap(GL_TEXTURE_2D);    // tạo trước có phiên bản zoom in/out
    } else std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);      // giải phóng data khi chép xong
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}