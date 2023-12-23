#include "game.h"
#include "graphics/resource_manager.h"
#include "graphics/sprite_renderer.h"
#include "game/game_object.h"
#include "game/ball_object.h"
#include "game/multi_ball.h"

#include <iostream>


//Game variables.
SpriteRenderer* renderer;
GameObject* paddle;
MultiBall* multiBall;

//Constructor for a Game with screen width and height.
Game::Game(unsigned int width, unsigned int height) {
    state = GAME_START;
    this->width = width;
    this->height = height;
}

//Destructor for a Game where all variables are deleted.
Game::~Game() {
    delete renderer;
    delete paddle;
    delete multiBall;
}

//Initialization function for Game variables and textures.
void Game::init() {
    //Load shader files.
    ResourceManager::loadShader("graphics/sprite.vs", "graphics/sprite.fs", nullptr, "sprite");

    //Set shader settings.
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width),
        static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    //Intialize renderer with shaders.
    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));


    //Load textures:
    //Start and end screen.
    ResourceManager::loadTexture("graphics/start_screen.jpg", false, "start_screen");
    ResourceManager::loadTexture("graphics/win_screen.jpg", false, "win_screen");

    //Level backgrounds.
    ResourceManager::loadTexture("graphics/crab_nebula.jpg", false, "crab_nebula");
    ResourceManager::loadTexture("graphics/pillars_of_creation.jpg", false, "pillars_of_creation");
    ResourceManager::loadTexture("graphics/ring_nebula.jpg", false, "ring_nebula");
    ResourceManager::loadTexture("graphics/carina_nebula.jpg", false, "carina_nebula");

    //Game objects.
    ResourceManager::loadTexture("graphics/ball.png", true, "ball");
    ResourceManager::loadTexture("graphics/block.jpg", false, "block");
    ResourceManager::loadTexture("graphics/paddle.png", true, "paddle");

    //Load levels.
    GameLevel* one = new GameLevel("levels/one.lvl", width, height / 2);
    GameLevel* two = new GameLevel("levels/two.lvl", width, height / 2);
    GameLevel* three = new GameLevel("levels/three.lvl", width, height / 2);
    GameLevel* four = new GameLevel("levels/four.lvl", width, height / 2);
    levels.push_back(*one);
    levels.push_back(*two);
    levels.push_back(*three);
    levels.push_back(*four);
    level = 0;

    //Create game objects.
    glm::vec2 paddlePos = glm::vec2(width / 2.0f - PADDLE_SIZE.x / 2.0f, height - PADDLE_SIZE.y);
    paddle = new GameObject(paddlePos, PADDLE_SIZE, ResourceManager::getTexture("paddle"));

    glm::vec2 ballPos = paddlePos + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    BallObject ball = *new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("ball"));
    multiBall = new MultiBall(ball);
}

//Update function that runs each frame.
void Game::update(float dt) {
    //Update the balls within the MultiBall object.
    MultiBall::Node* current = multiBall->head;
    MultiBall::Node* previous = nullptr;
    while (current) {
        BallObject* ball = &current->data;
        //Move each ball.
        ball->move(dt, width);
        //Check each ball for collisions.
        handleCollisions();
        //Check if the ball has exited the playzone.
        if (ball->position.y >= height) {
            if (!multiBall->head->next) {
                //If the ball is the only ball, reset the level.
                resetLevel();
                resetGameObjects();
                return;
            }

            if (&multiBall->head->data == ball) {
                //If the ball is the head, set the head to the next ball.
                multiBall->head = multiBall->head->next;
            } else {
                //Otherwise, remove the middle ball and set the previous ball's next to the current's next.
                previous->next = current->next;
                delete current;
                break;
            }
        }
        previous = current;
        current = current->next;
    }
}

//Function to process user input depending on game states.
void Game::processInput(float dt) {
    if (state == GAME_START) {
        //In the start state, we only check if the user presses space and start the game if they do.
        if (keys[GLFW_KEY_SPACE]) {
            state = GAME_ACTIVE;
        }
    } else if (state == GAME_ACTIVE) {
        //If the state is active, we want to check moving the paddle left and right, unsticking the ball, and changing the level.
        //Velocity is useful for updating the paddle and stuck ball positions.
        float velocity = PADDLE_VELOCITY * dt;

        //If the player inputs the A key, we want to move the paddle left.
        if (keys[GLFW_KEY_A]) {
            if (paddle->position.x >= 0.0f) {
                //If the paddle is not at the left wall, move it left.
                paddle->position.x -= velocity;

                //Check if there are any balls stuck to the paddle so we can move those left too.
                MultiBall::Node* iterator = multiBall->head;
                while (iterator) {
                    BallObject* ball = &iterator->data;
                    if (ball->stuck)
                        ball->position.x -= velocity;
                    iterator = iterator->next;
                }
            }
        }

        //If the player inputs the D key, we want to move the paddle right.
        if (keys[GLFW_KEY_D]) {
            if (paddle->position.x <= width - paddle->size.x) {
                //If the paddle is not at the right wall, move it right.
                paddle->position.x += velocity;

                //Check if there are any balls stuck to the paddle so we can move those right too.
                MultiBall::Node* iterator = multiBall->head;
                while (iterator) {
                    BallObject* ball = &iterator->data;
                    if (ball->stuck)
                        ball->position.x += velocity;
                    iterator = iterator->next;
                }
            }
        }

        //If the player inputs the Space key, we want to unstick any balls on the paddle.
        if (keys[GLFW_KEY_SPACE]) {
            MultiBall::Node* iterator = multiBall->head;
            while (iterator) {
                BallObject* ball = &iterator->data;
                ball->stuck = false;
                iterator = iterator->next;
            }
        }

        //If the player inputs the 1 key, we want to go to the first level. This is both for convenience and testing purposes.
        if (keys[GLFW_KEY_1]) {
            level = 0;
            resetLevel();
            resetGameObjects();
        }

        //If the player inputs the 2 key, we want to go to the second level. This is both for convenience and testing purposes.
        if (keys[GLFW_KEY_2]) {
            level = 1;
            resetLevel();
            resetGameObjects();
        }

        //If the player inputs the 3 key, we want to go to the third level. This is both for convenience and testing purposes.
        if (keys[GLFW_KEY_3]) {
            level = 2;
            resetLevel();
            resetGameObjects();
        }

        //If the player inputs the 4 key, we want to go to the fourth level. This is both for convenience and testing purposes.
        if (keys[GLFW_KEY_4]) {
            level = 3;
            resetLevel();
            resetGameObjects();
        }

        //If the player inputs the 5 key, we want to go to the win screen. This is for testing purposes.
        if (keys[GLFW_KEY_5]) {
            state = GAME_WIN;
        }
    }
}

//Rendering function for drawing the background, the level blocks, the paddle, and the balls.
void Game::render() {
    //Iterator for rendering each Ball in the MultiBall.
    MultiBall::Node* iterator = multiBall->head;

    //Switch based on the state enum value.
    switch (state) {
        case GAME_ACTIVE:
            //In the case of an active game, we want to draw the right background, the level blocks, the paddle, and the balls.
            switch (level) {
                case 0:
                    //If the level is 0, draw the crab nebula background.
                    renderer->drawSprite(ResourceManager::getTexture("crab_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
                    break;
                case 1:
                    //If the level is 1, draw the pillars of creation background.
                    renderer->drawSprite(ResourceManager::getTexture("pillars_of_creation"), glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
                    break;
                case 2:
                    //If the level is 2, draw the ring nebula background.
                    renderer->drawSprite(ResourceManager::getTexture("ring_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
                    break;
                case 3:
                    //If the level is 2, draw the carina nebula background.
                    renderer->drawSprite(ResourceManager::getTexture("carina_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
                    break;
            }

            //Draw the level blocks.
            levels[level].draw(*renderer);

            //Draw the paddle.
            paddle->draw(*renderer);

            //Draw each of the balls.
            while (iterator) {
                BallObject* ball = &iterator->data;
                ball->draw(*renderer);
                iterator = iterator->next;
            }
            break;
        case GAME_START:
            //In the case of the start state, draw the start screen background.
            renderer->drawSprite(ResourceManager::getTexture("start_screen"), glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
            break;
        case GAME_WIN:
            //In the case of the win state, draw the win screen background.
            renderer->drawSprite(ResourceManager::getTexture("win_screen"), glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
            break;
    }
}

//Function to reset the level if the player loses, the player beats a level, or the player wants to change levels.
void Game::resetLevel() {
    //Switch based on the value of the level.
    switch (level) {
        case 0:
            levels[0] = *new GameLevel("levels/one.lvl", width, height / 2);
            break;
        case 1:
            levels[1] = *new GameLevel("levels/two.lvl", width, height / 2);
            break;
        case 2:
            levels[2] = *new GameLevel("levels/three.lvl", width, height / 2);
            break;
        case 3:
            levels[3] = *new GameLevel("levels/four.lvl", width, height / 2);
            break;
    }
}

//Function to reset the game objects if the player loses, the player beats a level, or the player wants to change levels
void Game::resetGameObjects() {
    //Reste paddle size and position
    paddle->size = PADDLE_SIZE;
    paddle->position = glm::vec2(width / 2.0f - PADDLE_SIZE.x / 2.0f, height - PADDLE_SIZE.y);

    //Reset the MultiBall object to only have one resetted ball.
    multiBall->head->next = nullptr;
    BallObject& ball = multiBall->head->data;
    ball = *new BallObject((&ball)->position, BALL_RADIUS, (&ball)->velocity, ResourceManager::getTexture("ball"));
    (&ball)->reset(paddle->position + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

//Function to handle collisions.
void Game::handleCollisions() {
    //Iterate through each of the blocks in the current level.
    for (GameObject& block : levels[level].bricks) {
        if (!block.isDestroyed) {
            //If the block is not destroyed, we want to check if any ball collided with it.
            Collision collision;
            BallObject* collidedBall = new BallObject();
            MultiBall::Node* iterator = multiBall->head;

            //Iterate through each of the balls and check if there is a collision.
            while (iterator) {
                BallObject* ball = &iterator->data;
                collision = checkCollision(*ball, block);
                if (std::get<0>(collision)) {
                    collidedBall = ball;
                    break;
                }
                iterator = iterator->next;
            }

            //Check if there was a collision.
            if (std::get<0>(collision)) {
                

                //If there was a collision, destroy the block.
                block.isDestroyed = true;
                
                //Check if the block had special properties.
                
                //If the block is bouncy, set the speed multiplier to 1.1.
                float multiplier = 1;
                if (block.isBouncy)
                    multiplier = 1.1;

                //If the block is enlarging, double the radius of the ball object.
                if (block.isEnlarging) {
                    BallObject& collidedBallReference = *collidedBall;
                    collidedBallReference = *new BallObject(collidedBall->position, collidedBall->radius * 2, collidedBall->velocity, ResourceManager::getTexture("ball"));
                    collidedBall->stuck = false;
                }

                //If the block is cloning, add another ball to the MultiBall and set its position to that of the paddle.
                if (block.isCloning) {
                    glm::vec2 ballPos = paddle->position + glm::vec2(PADDLE_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
                    BallObject ball = *new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("ball"));
                    (&ball)->stuck = false;
                    multiBall->addFront(ball);
                }

                //Update the velocity of the ball depending on the direction of the collision.
                Direction dir = std::get<1>(collision);
                std::cout << dir;
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) {
                    //If the collision is left or right, reverse the ball's horizontal velocity.
                    collidedBall->velocity.x = -multiplier * collidedBall->velocity.x;
                    
                    //Move the ball to avoid multiple collisions
                    float penetration = collidedBall->radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        collidedBall->position.x += penetration;
                    else
                        collidedBall->position.x -= penetration;
                } else {
                    //If the collision is up or down, reverse the ball's vertical velocity.
                    collidedBall->velocity.y = -multiplier * collidedBall->velocity.y; // reverse vertical velocity

                    //Move the ball to avoid multiple collisions
                    float penetration = collidedBall->radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        collidedBall->position.y -= penetration;
                    else
                        collidedBall->position.y += penetration;
                }
            }
        }
    }

    //Check for collisions between the ball and the paddle.
    Collision result;
    BallObject* collidedBall = new BallObject();
    MultiBall::Node* iterator = multiBall->head;
    while (iterator) {
        BallObject* ball = &iterator->data;
        result = checkCollision(*ball, *paddle);
        if (!ball->stuck && std::get<0>(result)) {
            collidedBall = ball;
            break;
        }
        iterator = iterator->next;
    }

    //If there is a collision and the ball is not stuck to the paddle:
    if (!collidedBall->stuck && std::get<0>(result)) {
        //Update the ball's velocity based on where it hit the paddle.
        float centerBoard = paddle->position.x + paddle->size.x / 2.0f;
        float distance = (collidedBall->position.x + collidedBall->radius) - centerBoard;
        float percentage = distance / (paddle->size.x / 2.0f);
        float strength = 2.0f;
        glm::vec2 oldVelocity = collidedBall->velocity;
        collidedBall->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        collidedBall->velocity = glm::normalize(collidedBall->velocity) * glm::length(oldVelocity);

        //Reverse the ball's vertical velocity.
        collidedBall->velocity.y = -1.0f * abs(collidedBall->velocity.y);
    }
    
    //Check if all blocks are destroyed.
    bool won = true;
    for (GameObject& block : levels[level].bricks) {
        if (!block.isDestroyed) {
            won = false;
        }
    }
    if (won) {
        //If the level has been beaten, either reset and move on to the next or send the player to the win screen if it is the last level.
        if (level == 3) {
            state = GAME_WIN;
            return;
        }
        level = level + 1;
        resetLevel();
        resetGameObjects();
    }
}

//Helper function for seeing if there is a collision between two GameObjects.
bool Game::checkCollision(GameObject& one, GameObject& two) {
    //Separate the collision into its X and Y components.
    bool collisionX = one.position.x + one.size.x >= two.position.x && two.position.x + two.size.x >= one.position.x;
    bool collisionY = one.position.y + one.size.y >= two.position.y && two.position.y + two.size.y >= one.position.y;

    //Return true if it collided on both components.
    return collisionX && collisionY;
}

//Helper function for seeing if there is a collision between a Ball and a GameObject.
Collision Game::checkCollision(BallObject& one, GameObject& two) {
    glm::vec2 center(one.position + one.radius);
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.position.x + aabb_half_extents.x, two.position.y + aabb_half_extents.y);
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;

    if (glm::length(difference) < one.radius)
        return std::make_tuple(true, vectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

//Helper function for calculating which direction a vector is going.
Direction Game::vectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}