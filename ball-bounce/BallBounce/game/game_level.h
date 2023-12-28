#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "../graphics/sprite_renderer.h"
#include "../graphics/resource_manager.h"


//GameLevel contains the data of one level.
class GameLevel {
    public:
        //Tile data of the level.
        std::vector<std::vector<unsigned int>> tileData;

        //Bricks in the level.
        std::vector<GameObject> bricks;

        //Constructor based on file, width and height.
        GameLevel(const char* file, unsigned int levelWidth, unsigned int levelHeight);

        //Constructor for creating a random level.
        GameLevel(unsigned int levelWidth, unsigned int levelHeight);

        //Function to draw the level.
        void draw(SpriteRenderer& renderer);

        //Function to see if the level is completed.
        bool isCompleted();

    private:
        //Function to initialize the level.
        void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif