#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game/game_level.h"
#include "game/ball_object.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_START,
    GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// Initial size of the player paddle
const glm::vec2 PADDLE_SIZE(150.0f, 20.0f);
// Initial velocity of the player paddle
const float PADDLE_VELOCITY(500.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 25.0f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
    public:
        // game state
        GameState state;
        bool keys[1024];
        unsigned int width, height;
        std::vector<GameLevel> levels;
        unsigned int level;
        // constructor/destructor
        Game(unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void init();
        // game loop
        void processInput(float dt);
        void update(float dt);
        void render();
        
        // reset
        void resetLevel();
        void resetGameObjects();

        //Collision functions.
        void handleCollisions();
        bool checkCollision(GameObject& one, GameObject& two);
        Collision checkCollision(BallObject& one, GameObject& two);
        Direction vectorDirection(glm::vec2 closest);
};

#endif