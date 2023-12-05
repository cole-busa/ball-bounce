#include "game_level.h"

#include <fstream>
#include <sstream>
#include <iostream>


void GameLevel::load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
    // clear old data
    this->bricks.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream) {
        while (std::getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::draw(SpriteRenderer& renderer) {
    for (GameObject& tile : this->bricks)
        if (!tile.destroyed)
            tile.draw(renderer);
}

bool GameLevel::isCompleted() {
    for (GameObject& tile : this->bricks)
        if (!tile.isSolid && !tile.destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = 80;
    float unit_height = 50;
    // initialize level tiles based on tileData		
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::getTexture("block"), glm::vec3(1.0f, 0.75f, 0.8f));
                obj.isBouncy = true;
                this->bricks.push_back(obj);
            } else if (tileData[y][x] == 2) {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size, ResourceManager::getTexture("block"), glm::vec3(0.0f, 1.0f, 1.0f));
                obj.isEnlarging = true;
                this->bricks.push_back(obj);
            } else if (tileData[y][x] > 2) {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 3)
                    color = glm::vec3(1.0f, 0.0f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.0f, 1.0f, 0.0f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(0.0f, 0.0f, 1.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->bricks.push_back(GameObject(pos, size, ResourceManager::getTexture("block"), color));
            }
        }
    }
}