#include "game_level.h"

#include <fstream>
#include <sstream>
#include <iostream>


//Constructor based on file, width and height.
GameLevel::GameLevel(const char* file, unsigned int levelWidth, unsigned int levelHeight) {
    //Clear old data.
    bricks.clear();

    //Load from file:
    unsigned int tileCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream) {
        while (std::getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            //Read each line separated by spaces.
            while (sstream >> tileCode)
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            init(tileData, levelWidth, levelHeight);
    }
}

//Function to draw the level.
void GameLevel::draw(SpriteRenderer& renderer) {
    //Draw only tiles that are not destroyed.
    for (GameObject& tile : bricks)
        if (!tile.destroyed)
            tile.draw(renderer);
}

//Function to see if the level is completed.
bool GameLevel::isCompleted() {
    //Return true if all tiles are destroyed, otherwise return false.
    for (GameObject& tile : this->bricks)
        if (!tile.destroyed)
            return false;
    return true;
}

//Function to initialize the level.
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
            } else if (tileData[y][x] == 2) {
                //A tile value of 1 is a Enlarging Tile. It will double the radius of the ball.
                color = glm::vec3(0.0f, 1.0f, 1.0f);
                type = "enlarging";
            } else if (tileData[y][x] == 3) {
                //A tile value of 1 is a Cloning Tile. It will spawn a new ball at the paddle position.
                color = glm::vec3(1.0f, 0.0f, 0.0f);
                type = "cloning";
            } else if (tileData[y][x] == 4) {
                color = glm::vec3(0.0f, 1.0f, 0.0f); 
            } else if (tileData[y][x] == 5) {
                color = glm::vec3(0.0f, 0.0f, 1.0f);
            } else if (tileData[y][x] == 6) {
                color = glm::vec3(1.0f, 1.0f, 1.0f);
            } else {
                continue;
            }
            
            //Create the Brick Game Object based on the previous information. Add it to the list of bricks.
            GameObject obj(pos, size, ResourceManager::getTexture("block"), color, glm::vec2(0.0f, 0.0f), type);
            bricks.push_back(obj);
        }
    }
}