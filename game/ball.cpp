#include "ball.h"
#include <glm/glm.hpp>

Ball::Ball(): GameObject(), Radius(12.5f), Stuck(true) {}

Ball::Ball(glm::vec2 pos, float radius, glm::vec2 velocity)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), velocity), Radius(radius), Stuck(true) {}

glm::vec2 Ball::Move(float dt, unsigned int window_width) {
    if (!this->Stuck) {
        // move the ball
        this->Position += this->Velocity * dt;
        // check if outside window bounds; if so, reverse velocity and restore at correct position
        if (this->Position.x <= 0.0f) {
            this->Velocity.x = - this->Velocity.x;
            this->Position.x = 0.0f;
        } else if (this->Position.x + this->Size.x >= window_width) {
            this->Velocity.x = - this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (this->Position.y <= 0.0f) {
            this->Velocity.y = - this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    return this->Position;
}

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void Ball::Reset(glm::vec2 position, glm::vec2 velocity) {
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}