#ifndef GAME_H_
#define GAME_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "sprite_renderer.h"
#include "game_level.h"
#include "ball.h"
#include "game_object.h"

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
private:
    GameLevel       Level;
    GameObject      Player;
    Ball            BallObj;
    GameState       State;
    unsigned int    Width, Height;
public:
    bool            Keys[1024];

    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
};

#endif