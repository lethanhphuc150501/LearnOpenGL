#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // for glm::ortho, glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "sprite_renderer.h"
#include "ball.h"

static SpriteRenderer* Renderer = NULL;
static Shader* sprite = NULL;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);
GameObject *Player = NULL;

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;
Ball *BallObj = NULL;

Game::Game(unsigned int width, unsigned int height): State(GAME_ACTIVE), Keys(), Width(width), Height(height) {

}

Game::~Game() {
    if (sprite != NULL) delete sprite;
    if (Renderer != NULL) delete Renderer;
    if (Player != NULL) delete Player;
    if (BallObj != NULL) delete BallObj;
}

void Game::Init() {
    sprite = new Shader("sprite.vs", "sprite.fs");
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
    sprite->use();
    sprite->setMat4("projection", proj);
    Renderer = new SpriteRenderer(sprite);

    this->Level.Load("level_data", Width, Height / 2);

    glm::vec2 playerPos = glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f, Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, - BALL_RADIUS * 2.0f);
    BallObj = new Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY);
}

void Game::ProcessInput(float dt) {
    if (State == GAME_ACTIVE) {
        float dx = PLAYER_VELOCITY * dt;
        if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]) {
            if (Player->Position.x >= 0.0f) {
                Player->Position.x -= dx;
                if (BallObj->Stuck) BallObj->Position.x -= dx;
            }
        }
        if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]) {
            if (Player->Position.x <= Width - Player->Size.x) {
                Player->Position.x += dx;
                if (BallObj->Stuck) BallObj->Position.x += dx;
            }
        }
        if (Keys[GLFW_KEY_SPACE]) BallObj->Stuck = false;
    }
}

void Game::Update(float dt) {
    BallObj->Move(dt, Width);
}

void Game::Render() {
    if (State == GAME_ACTIVE) {
        this->Level.Draw(*Renderer);
        Player->Draw(*Renderer);
        BallObj->Draw(*Renderer);
    }
}