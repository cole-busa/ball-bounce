#include "game.h"
#include "graphics/resource_manager.h"
#include "graphics/sprite_renderer.h"
#include "game/game_object.h"
#include "game/ball_object.h"
#include "game/multi_ball.h"

#include <iostream>

// Game-related State data
SpriteRenderer* renderer;
GameObject* player;
MultiBall* multiBall;

Game::Game(unsigned int width, unsigned int height) {
    this->state = GAME_MENU;
    this->keys;
    this->width = width;
    this->height = height;
}

Game::~Game() {
    delete renderer;
    delete player;
    delete multiBall;
}

void Game::init() {
    // load shaders
    ResourceManager::loadShader("graphics/sprite.vs", "graphics/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    // set render-specific controls
    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
    // load textures
    ResourceManager::loadTexture("graphics/start_screen.jpg", false, "start_screen");
    ResourceManager::loadTexture("graphics/crab_nebula.jpg", false, "crab_nebula");
    ResourceManager::loadTexture("graphics/pillars_of_creation.jpg", false, "pillars_of_creation");
    ResourceManager::loadTexture("graphics/ring_nebula.jpg", false, "ring_nebula");
    ResourceManager::loadTexture("graphics/carina_nebula.jpg", false, "carina_nebula");
    ResourceManager::loadTexture("graphics/ball.png", true, "ball");
    ResourceManager::loadTexture("graphics/block.jpg", false, "block");
    ResourceManager::loadTexture("graphics/paddle.png", true, "paddle");
    // load levels
    GameLevel one; one.load("levels/one.lvl", this->width, this->height / 2);
    GameLevel two; two.load("levels/two.lvl", this->width, this->height / 2);
    GameLevel three; three.load("levels/three.lvl", this->width, this->height / 2);
    GameLevel four; four.load("levels/four.lvl", this->width, this->height / 2);
    this->levels.push_back(one);
    this->levels.push_back(two);
    this->levels.push_back(three);
    this->levels.push_back(four);
    this->level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    BallObject ball = *new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("ball"));
    multiBall = new MultiBall(ball);
}

void Game::update(float dt) {
    // update objects
    MultiBall::Node* current = multiBall->head;
    MultiBall::Node* previous = nullptr;
    while (current) {
        BallObject* ball = &current->data;
        ball->move(dt, this->width);
        // check for collisions
        this->doCollisions();
        // check loss condition
        if (ball->position.y >= this->height) {
            if (!multiBall->head->next) {
                this->resetLevel();
                this->resetPlayer();
                return;
            }

            if (&multiBall->head->data == ball) {
                multiBall->head = multiBall->head->next;
            } else {
                previous->next = current->next;
                delete current;
                break;
            }
        }
        if (this->state == GAME_WIN) {
            this->state = GAME_ACTIVE;
            this->level = this->level + 1;
            this->resetLevel();
            this->resetPlayer();
        }
        previous = current;
        current = current->next;
    }
}

void Game::processInput(float dt) {
    if (this->state == GAME_MENU) {
        if (this->keys[GLFW_KEY_SPACE]) {
            this->state = GAME_ACTIVE;
        }
    } else if (this->state == GAME_ACTIVE) {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->keys[GLFW_KEY_A]) {
            if (player->position.x >= 0.0f) {
                player->position.x -= velocity;

                MultiBall::Node* temp = multiBall->head;
                while (temp) {
                    BallObject* ball = &temp->data;
                    if (ball->stuck)
                        ball->position.x -= velocity;
                    temp = temp->next;
                }
            }
        }
        if (this->keys[GLFW_KEY_D]) {
            if (player->position.x <= this->width - player->size.x) {
                player->position.x += velocity;

                MultiBall::Node* temp = multiBall->head;
                while (temp) {
                    BallObject* ball = &temp->data;
                    if (ball->stuck)
                        ball->position.x += velocity;
                    temp = temp->next;
                }
            }
        }
        if (this->keys[GLFW_KEY_SPACE]) {
            MultiBall::Node* temp = multiBall->head;
            while (temp) {
                BallObject* ball = &temp->data;
                ball->stuck = false;
                temp = temp->next;
            }
        }
        if (this->keys[GLFW_KEY_1]) {
            this->level = 0;
            this->resetLevel();
            this->resetPlayer();
        }
        if (this->keys[GLFW_KEY_2]) {
            this->level = 1;
            this->resetLevel();
            this->resetPlayer();
        }
        if (this->keys[GLFW_KEY_3]) {
            this->level = 2;
            this->resetLevel();
            this->resetPlayer();
        }
        if (this->keys[GLFW_KEY_4]) {
            this->level = 3;
            this->resetLevel();
            this->resetPlayer();
        }
    }
}

void Game::render() {
    if (this->state == GAME_MENU) {
        renderer->drawSprite(ResourceManager::getTexture("start_screen"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
    } else if (this->state == GAME_ACTIVE) {
        // draw background
        if (this->level == 0)
            renderer->drawSprite(ResourceManager::getTexture("crab_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 1)
            renderer->drawSprite(ResourceManager::getTexture("pillars_of_creation"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 2)
            renderer->drawSprite(ResourceManager::getTexture("ring_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 3)
            renderer->drawSprite(ResourceManager::getTexture("carina_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 4)
            renderer->drawSprite(ResourceManager::getTexture("carina_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        // draw level
        this->levels[this->level].draw(*renderer);
        // draw player
        player->draw(*renderer);
        // draw ball
        MultiBall::Node* temp = multiBall->head;
        while (temp) {
            BallObject* ball = &temp->data;
            ball->draw(*renderer);
            temp = temp->next;
        }
    }
}


void Game::resetLevel() {
    if (this->level == 0)
        this->levels[0].load("levels/one.lvl", this->width, this->height / 2);
    else if (this->level == 1)
        this->levels[1].load("levels/two.lvl", this->width, this->height / 2);
    else if (this->level == 2)
        this->levels[2].load("levels/three.lvl", this->width, this->height / 2);
    else if (this->level == 3)
        this->levels[3].load("levels/four.lvl", this->width, this->height / 2);
}

void Game::resetPlayer() {
    // reset player/ball stats
    player->size = PLAYER_SIZE;
    player->position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    multiBall->head->next = nullptr;
    BallObject& ball = multiBall->head->data;
    ball = *new BallObject((&ball)->position, BALL_RADIUS, (&ball)->velocity, ResourceManager::getTexture("ball"));
    (&ball)->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

// collision detection
bool checkCollision(GameObject& one, GameObject& two);
Collision checkCollision(BallObject& one, GameObject& two);
Direction vectorDirection(glm::vec2 closest);

void Game::doCollisions() {
    for (GameObject& box : this->levels[this->level].bricks) {
        if (!box.destroyed) {
            Collision collision;
            BallObject* collidedBall = new BallObject();
            MultiBall::Node* temp = multiBall->head;
            while (temp) {
                BallObject* ball = &temp->data;
                collision = checkCollision(*ball, box);
                if (std::get<0>(collision)) {
                    collidedBall = ball;
                    break;
                }
                temp = temp->next;
            }
            if (std::get<0>(collision)) {
                float multiplier = 1;

                // destroy block if not solid
                if (!box.isSolid)
                    box.destroyed = true;
                //Check if bouncy
                if (box.isBouncy)
                    multiplier = 1.1;
                //Check if enlarging
                if (box.isEnlarging) {
                    BallObject& collidedBallReference = *collidedBall;
                    collidedBallReference = *new BallObject(collidedBall->position, collidedBall->radius * 2, collidedBall->velocity, ResourceManager::getTexture("ball"));
                    collidedBall->stuck = false;
                }
                //Check if cloning
                if (box.isCloning) {
                    glm::vec2 ballPos = player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
                    BallObject ball = *new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("ball"));
                    (&ball)->stuck = false;
                    multiBall->addFront(ball);
                }

                // collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) {
                    collidedBall->velocity.x = -multiplier * collidedBall->velocity.x; // reverse horizontal velocity
                    // relocate
                    float penetration = collidedBall->radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        collidedBall->position.x += penetration; // move ball to right
                    else
                        collidedBall->position.x -= penetration; // move ball to left;
                }
                else {
                    collidedBall->velocity.y = -multiplier * collidedBall->velocity.y; // reverse vertical velocity
                    // relocate
                    float penetration = collidedBall->radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        collidedBall->position.y -= penetration; // move ball bback up
                    else
                        collidedBall->position.y += penetration; // move ball back down
                }
            }
        }
    }
    // check collisions for player pad (unless stuck)
    Collision result;
    BallObject* collidedBall = new BallObject();
    MultiBall::Node* temp = multiBall->head;
    while (temp) {
        BallObject* ball = &temp->data;
        result = checkCollision(*ball, *player);
        if (!ball->stuck && std::get<0>(result)) {
            collidedBall = ball;
            break;
        }
        temp = temp->next;
    }
    if (!collidedBall->stuck && std::get<0>(result)) {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = player->position.x + player->size.x / 2.0f;
        float distance = (collidedBall->position.x + collidedBall->radius) - centerBoard;
        float percentage = distance / (player->size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = collidedBall->velocity;
        collidedBall->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        collidedBall->velocity = glm::normalize(collidedBall->velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        // fix sticky paddle
        collidedBall->velocity.y = -1.0f * abs(collidedBall->velocity.y);
    }
    
    //Check if all blocks are destroyed
    bool won = true;
    for (GameObject& box : this->levels[this->level].bricks) {
        if (!box.destroyed) {
            won = false;
        }
    }
    if (won) {
        this->state = GAME_WIN;
    }
}

bool checkCollision(GameObject& one, GameObject& two) {
    // collision x-axis?
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
        two.position.x + two.size.x >= one.position.x;
    // collision y-axis?
    bool collisionY = one.position.y + one.size.y >= two.position.y &&
        two.position.y + two.size.y >= one.position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

Collision checkCollision(BallObject& one, GameObject& two) {
    // get center point circle first 
    glm::vec2 center(one.position + one.radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.position.x + aabb_half_extents.x, two.position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (glm::length(difference) < one.radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, vectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

// calculates which direction a vector is facing (N,E,S or W)
Direction vectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
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