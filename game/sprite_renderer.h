#ifndef SPRITE_RENDERER_H_
#define SPRITE_RENDERER_H_

#include "../shader.h"
#include <glm/glm.hpp>

class SpriteRenderer {
private:
    // Singleton pattern
    static SpriteRenderer* instance;

    Shader* shader;
    unsigned int quadVAO;
    void initRenderData();
    SpriteRenderer();
    static SpriteRenderer* getRenderer();
public:
    ~SpriteRenderer();
    static void SetProjection(unsigned int width, unsigned int height);
    static void DrawSprite(glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), glm::vec3 color = glm::vec3(1.0f));
};

#endif