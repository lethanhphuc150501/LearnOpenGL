#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // for glm::ortho, glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "sprite_renderer.h"

static SpriteRenderer* Renderer = NULL;
static Shader* sprite = NULL;

Game::Game(unsigned int width, unsigned int height): State(GAME_ACTIVE), Keys(), Width(width), Height(height) {

}

Game::~Game() {
    if (sprite != NULL) delete sprite;
    if (Renderer != NULL) delete Renderer;
}

void Game::Init() {
    sprite = new Shader("sprite.vs", "sprite.fs");
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
    sprite->use();
    sprite->setMat4("projection", proj);
    Renderer = new SpriteRenderer(sprite);

    this->Level.Load("level_data", Width, Height / 2);
}

void Game::ProcessInput(float dt) {

}

void Game::Update(float dt) {

}

void Game::Render() {
    if (State == GAME_ACTIVE) {
        this->Level.Draw(*Renderer);
    }
}