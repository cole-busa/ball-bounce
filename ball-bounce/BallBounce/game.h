#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game/game_level.h"
#include "game/ball_object.h"


//Enum for the current game state. Active is the playing state, start is the start screen, and win is the win screen.
enum GameState {
    GAME_ACTIVE,
    GAME_START,
    GAME_WIN,
    GAME_RANDOM
};

//Enum for the directions a collision can take.
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

//Collision type that contains if a collision happened, the direction it happened in, and the size of the collision.
typedef std::tuple<bool, Direction, glm::vec2> Collision;

//Size of the paddle.
const glm::vec2 PADDLE_SIZE(150.0f, 20.0f);
//Velocity of the paddle.
const float PADDLE_VELOCITY(500.0f);
//Starting velocity of the ball.
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
//Starting radius of the ball.
const float BALL_RADIUS = 25.0f;

//Game contains all methods for the functionality of the game.
class Game {
    public:
        //Game state.
        GameState state;

        //Array for the inputs a player can give.
        bool keys[1024];

        //Width and height of the screen.
        unsigned int width, height;

        //Contains brick level data.
        std::vector<GameLevel> levels;

        //Level that the player is on.
        unsigned int level;

        //Score of the player.
        int score;

        //Constructor and destructor.
        Game(unsigned int width, unsigned int height);
        ~Game();

        //Initialization function.
        void init();

        //Input processing function.
        void processInput(float dt);

        //Update and render functions.
        void update(float dt);
        void render();
        
        //Reset functions.
        void resetLevel();
        void resetGameObjects();

        //Collision functions.
        void handleCollisions();
        bool checkCollision(GameObject& one, GameObject& two);
        Collision checkCollision(BallObject& one, GameObject& two);
        Direction vectorDirection(glm::vec2 closest);
};

#endif