#include <fstream>
#include <sstream>
#include "game_level.h"
#include "game_object.h"
#include "sprite_renderer.h"


void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight) {
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream) {
        while (std::getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
    // calculate dimensions
    float unit_width  = levelWidth  / static_cast<float>(tileData[0].size());
    float unit_height = levelHeight / static_cast<float>(tileData.size());
    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < tileData.size(); ++y) {
        for (unsigned int x = 0; x < tileData[0].size(); ++x) {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, glm::vec3(0.8f, 0.8f, 0.7f));
                this->Bricks.push_back(obj);
            }
        }
    }
}

void GameLevel::Draw(SpriteRenderer &renderer) {
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed) tile.Draw(renderer);
}