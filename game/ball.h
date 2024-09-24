#ifndef BALL_H_
#define BALL_H_

#include <glm/glm.hpp>
#include "game_object.h"

class Ball : public GameObject {
public:
    float   Radius;
    bool    Stuck;

    Ball();
    Ball(glm::vec2 pos, float radius, glm::vec2 velocity);

    glm::vec2 Move(float dt, unsigned int window_width);
    void      Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif