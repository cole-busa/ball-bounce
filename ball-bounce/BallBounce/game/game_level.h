#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "../graphics/sprite_renderer.h"
#include "../graphics/resource_manager.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel {
    public:
        // level state
        std::vector<GameObject> bricks;
        // constructor
        GameLevel() { }
        // loads level from file
        void load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
        // render level
        void draw(SpriteRenderer& renderer);
        // check if the level is completed (all non-solid tiles are destroyed)
        bool isCompleted();
    private:
        // initialize level from tile data
        void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif