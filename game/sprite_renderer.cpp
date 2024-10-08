#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "sprite_renderer.h"

SpriteRenderer* SpriteRenderer::instance = nullptr;

SpriteRenderer::SpriteRenderer() {
    this->shader = new Shader("sprite.vs", "sprite.fs");
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
    delete this->shader;
}

SpriteRenderer* SpriteRenderer::getRenderer() {
    if (!instance) instance = new SpriteRenderer();
    return instance;
}

Shader* SpriteRenderer::getShader() {
    return this->shader;
}

void SpriteRenderer::initRenderData() {
    unsigned int VBO;
    float vertices[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindVertexArray(this->quadVAO);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::DrawSprite(glm::vec2 position, glm::vec2 size, glm::vec3 color) {
    this->shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0));
    model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    shader->setMat4("model", model);
    shader->setVec3("spriteColor", color);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_QUADS, 0, 4);
    glBindVertexArray(0);
}