#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // for glm::ortho, glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
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

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
typedef std::tuple<bool, Direction> Collision;
Collision CheckCollision(Ball &ball, GameObject &obj);
Direction VectorDirection(glm::vec2 target);

Game::Game(unsigned int width, unsigned int height): State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

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
    this->DoCollisions();
}

void Game::Render() {
    if (State == GAME_ACTIVE) {
        this->Level.Draw(*Renderer);
        Player->Draw(*Renderer);
        BallObj->Draw(*Renderer);
    }
}

void Game::DoCollisions() {
    for (GameObject &box : this->Level.Bricks) {
        if (!box.Destroyed) {
            Collision check = CheckCollision(*BallObj, box);
            if (std::get<0>(check)) {
                box.Destroyed = true;
                Direction dir = std::get<1>(check);
                if (dir == LEFT || dir == RIGHT) BallObj->Velocity.x = - BallObj->Velocity.x; // reverse x
                else BallObj->Velocity.y = - BallObj->Velocity.y; // reverse y
            }
        }
    }
    Collision result = CheckCollision(*BallObj, *Player);
    if (!BallObj->Stuck && std::get<0>(result)) {
        // check where it hit the board, and change velocity
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float centerBall  = BallObj->Position.x + BallObj->Radius;
        float distance = centerBall - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = BallObj->Velocity;
        BallObj->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        BallObj->Velocity.y = - 1.0f * abs(BallObj->Velocity.y);
        BallObj->Velocity = glm::normalize(BallObj->Velocity) * glm::length(oldVelocity);
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