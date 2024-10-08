#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <vector>
#include "game_object.h"
#include "sprite_renderer.h"

class GameLevel {
public:
    // level state
    std::vector<GameObject> Bricks;
    // constructor
    GameLevel() {}
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw();
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif