#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // for glm::ortho, glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
#include "game.h"
#include "sprite_renderer.h"
#include "ball.h"

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
typedef std::tuple<bool, Direction> Collision;

static Collision CheckCollision(Ball &ball, GameObject &obj);
static Direction VectorDirection(glm::vec2 target);

Game::Game(unsigned int width, unsigned int height) {
    this->State = GAME_ACTIVE;
    this->Width = width;
    this->Height = height;

    glm::vec2 playerPos = glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f, Height - PLAYER_SIZE.y);
    this->Player = GameObject(playerPos, PLAYER_SIZE, glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, - BALL_RADIUS * 2.0f);
    this->BallObj = Ball(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY);
}

Game::~Game() {}

void Game::Init() {
    SpriteRenderer* Renderer = SpriteRenderer::getRenderer();
    Shader* sprite = Renderer->getShader();
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
    sprite->use();
    sprite->setMat4("projection", proj);

    this->Level.Load("level_data", Width, Height / 2);
}

void Game::ProcessInput(float dt) {
    if (State == GAME_ACTIVE) {
        float dx = PLAYER_VELOCITY * dt;
        if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]) {
            if (Player.Position.x >= 0.0f) {
                Player.Position.x -= dx;
                if (BallObj.Stuck) BallObj.Position.x -= dx;
            }
        }
        if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]) {
            if (Player.Position.x <= Width - Player.Size.x) {
                Player.Position.x += dx;
                if (BallObj.Stuck) BallObj.Position.x += dx;
            }
        }
        if (Keys[GLFW_KEY_SPACE]) BallObj.Stuck = false;
    }
}

void Game::Update(float dt) {
    BallObj.Move(dt, Width);
    this->DoCollisions();
    if (BallObj.Position.y >= Height) {
        this->Level.Load("level_data", this->Width, this->Height / 2);
        Player.Size = PLAYER_SIZE;
        Player.Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
        BallObj.Reset(Player.Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
    }
}

void Game::Render() {
    if (State == GAME_ACTIVE) {
        this->Level.Draw();
        Player.Draw();
        BallObj.Draw();
    }
}

void Game::DoCollisions() {
    for (GameObject &box : this->Level.Bricks) {
        if (!box.Destroyed) {
            Collision check = CheckCollision(BallObj, box);
            if (std::get<0>(check)) {
                box.Destroyed = true;
                Direction dir = std::get<1>(check);
                if (dir == LEFT || dir == RIGHT) BallObj.Velocity.x = - BallObj.Velocity.x; // reverse x
                else BallObj.Velocity.y = - BallObj.Velocity.y; // reverse y
            }
        }
    }
    Collision result = CheckCollision(BallObj, Player);
    if (!BallObj.Stuck && std::get<0>(result)) {
        // check where it hit the board, and change velocity
        float centerBoard = Player.Position.x + Player.Size.x / 2.0f;
        float centerBall  = BallObj.Position.x + BallObj.Radius;
        float distance = centerBall - centerBoard;
        float percentage = distance / (Player.Size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = BallObj.Velocity;
        BallObj.Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        BallObj.Velocity.y = - 1.0f * abs(BallObj.Velocity.y);
        BallObj.Velocity = glm::normalize(BallObj.Velocity) * glm::length(oldVelocity);
    }
}

Collision CheckCollision(Ball &ball, GameObject &obj) {
    bool collisionX = ball.Position.x + ball.Size.x >= obj.Position.x && obj.Position.x + obj.Size.x >= ball.Position.x;
    bool collisionY = ball.Position.y + ball.Size.y >= obj.Position.y && obj.Position.y + obj.Size.y >= ball.Position.y;
    
    glm::vec2 center(ball.Position + ball.Radius);
    glm::vec2 aabb_center = obj.Position + glm::vec2(obj.Size.x / 2.0f, obj.Size.y / 2.0f);
    glm::vec2 difference = center - aabb_center;

    if (collisionX && collisionY)
        return std::make_tuple(true, VectorDirection(difference));
    else
        return std::make_tuple(false, UP);
}

Direction VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f), // up
        glm::vec2(1.0f, 0.0f), // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f) // left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}