#include "game_level.h"

#include <fstream>
#include <sstream>
#include <iostream>


GameLevel::GameLevel(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
    // clear old data
    this->bricks.clear();
    // load from file
    unsigned int tileCode;
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
        if (!tile.destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {
    //Playfield dimensions.
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unit_width = 80;
    float unit_height = 50;

    //Initialize level tiles based on tileData.	
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            //Default color, position, size, and type.
            glm::vec3 color = glm::vec3(1.0f);
            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width, unit_height);
            std::string type = "none";

            //Check block type:
            if (tileData[y][x] == 1) {
                //A tile value of 1 is a Bouncy Tile. It will increase the velocity of the ball more than a normal tile.
                color = glm::vec3(1.0f, 0.75f, 0.8f);
                type = "bouncy";
            }
            else if (tileData[y][x] == 2) {
                //A tile value of 1 is a Enlarging Tile. It will double the radius of the ball.
                color = glm::vec3(0.0f, 1.0f, 1.0f);
                type = "enlarging";
            }
            else if (tileData[y][x] == 3) {
                //A tile value of 1 is a Cloning Tile. It will spawn a new ball at the paddle position.
                color = glm::vec3(0.5f, 0.5f, 0.5f);
                type = "cloning";
            } else if (tileData[y][x] == 3) {
                color = glm::vec3(1.0f, 0.0f, 0.0f);
            } else if (tileData[y][x] == 4) {
                color = glm::vec3(0.0f, 1.0f, 0.0f); 
            } else if (tileData[y][x] == 5) {
                color = glm::vec3(0.0f, 0.0f, 1.0f);
            } else {
                continue;
            }
            
            GameObject obj(pos, size, ResourceManager::getTexture("block"), color, glm::vec2(0.0f, 0.0f), type);
            bricks.push_back(obj);
        }
    }
}