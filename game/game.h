#ifndef GAME_H_
#define GAME_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "sprite_renderer.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
    public:
        GameState       State;
        bool            Keys[1024];
        unsigned int    Width, Height;
        Game(unsigned int width, unsigned int height);
        ~Game();

        void Init();
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
};

#endif