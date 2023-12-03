#include "game.h"
#include "graphics/resource_manager.h"
#include "graphics/sprite_renderer.h"
#include "game/game_object.h"
#include "game/ball_object.h"

#include <iostream>

// Game-related State data
SpriteRenderer* renderer;
GameObject* player;
BallObject* ball;

Game::Game(unsigned int width, unsigned int height) {
    this->state = GAME_ACTIVE;
    this->keys;
    this->width = width;
    this->height = height;
}

Game::~Game() {
    delete renderer;
    delete player;
    delete ball;
}

void Game::init() {
    // load shaders
    ResourceManager::LoadShader("graphics/sprite.vs", "graphics/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("graphics/crab_nebula.jpg", false, "crab_nebula");
    ResourceManager::LoadTexture("graphics/pillars_of_creation.jpg", false, "pillars_of_creation");
    ResourceManager::LoadTexture("graphics/ring_nebula.jpg", false, "ring_nebula");
    ResourceManager::LoadTexture("graphics/carina_nebula.jpg", false, "carina_nebula");
    ResourceManager::LoadTexture("graphics/ball.png", true, "ball");
    ResourceManager::LoadTexture("graphics/block.jpg", false, "block");
    ResourceManager::LoadTexture("graphics/paddle.png", true, "paddle");
    // load levels
    GameLevel one; one.Load("levels/one.lvl", this->width, this->height / 2);
    GameLevel two; two.Load("levels/two.lvl", this->width, this->height / 2);
    GameLevel three; three.Load("levels/three.lvl", this->width, this->height / 2);
    GameLevel four; four.Load("levels/four.lvl", this->width, this->height / 2);
    this->levels.push_back(one);
    this->levels.push_back(two);
    this->levels.push_back(three);
    this->levels.push_back(four);
    this->level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
}

void Game::update(float dt) {
    // update objects
    ball->Move(dt, this->width);
    // check for collisions
    this->doCollisions();
    // check loss condition
    if (ball->Position.y >= this->height) {
        this->resetLevel();
        this->resetPlayer();
    }
    if (this->state == GAME_WIN) {
        this->state = GAME_ACTIVE;
        this->level = this->level + 1;
        this->resetLevel();
        this->resetPlayer();
    }
}

void Game::processInput(float dt) {
    if (this->state == GAME_ACTIVE) {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->keys[GLFW_KEY_A]) {
            if (player->Position.x >= 0.0f) {
                player->Position.x -= velocity;
                if (ball->Stuck)
                    ball->Position.x -= velocity;
            }
        }
        if (this->keys[GLFW_KEY_D]) {
            if (player->Position.x <= this->width - player->Size.x) {
                player->Position.x += velocity;
                if (ball->Stuck)
                    ball->Position.x += velocity;
            }
        }
        if (this->keys[GLFW_KEY_SPACE]) {
            ball->Stuck = false;
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
    if (this->state == GAME_ACTIVE) {
        // draw background
        if (this->level == 0)
            renderer->DrawSprite(ResourceManager::GetTexture("crab_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 1)
            renderer->DrawSprite(ResourceManager::GetTexture("pillars_of_creation"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 2)
            renderer->DrawSprite(ResourceManager::GetTexture("ring_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 3)
            renderer->DrawSprite(ResourceManager::GetTexture("carina_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        else if (this->level == 4)
            renderer->DrawSprite(ResourceManager::GetTexture("carina_nebula"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        // draw level
        this->levels[this->level].Draw(*renderer);
        // draw player
        player->Draw(*renderer);
        // draw ball
        ball->Draw(*renderer);
    }
}


void Game::resetLevel() {
    if (this->level == 0)
        this->levels[0].Load("levels/one.lvl", this->width, this->height / 2);
    else if (this->level == 1)
        this->levels[1].Load("levels/two.lvl", this->width, this->height / 2);
    else if (this->level == 2)
        this->levels[2].Load("levels/three.lvl", this->width, this->height / 2);
    else if (this->level == 3)
        this->levels[3].Load("levels/four.lvl", this->width, this->height / 2);
}

void Game::resetPlayer() {
    // reset player/ball stats
    player->Size = PLAYER_SIZE;
    player->Position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    ball = new BallObject(ball->Position, BALL_RADIUS, ball->Velocity, ResourceManager::GetTexture("ball"));
    ball->Reset(player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

// collision detection
bool checkCollision(GameObject& one, GameObject& two);
Collision checkCollision(BallObject& one, GameObject& two);
Direction vectorDirection(glm::vec2 closest);

void Game::doCollisions() {
    for (GameObject& box : this->levels[this->level].Bricks) {
        if (!box.Destroyed) {
            Collision collision = checkCollision(*ball, box);
            if (std::get<0>(collision)) {
                float multiplier = 1;

                // destroy block if not solid
                if (!box.IsSolid)
                    box.Destroyed = true;
                //Check if bouncy
                if (box.IsBouncy)
                    multiplier = 1.1;
                //Check if clone
                if (box.IsEnlarging) {
                    ball = new BallObject(ball->Position, ball->Radius * 2, ball->Velocity, ResourceManager::GetTexture("ball"));
                    ball->Stuck = false;
                }

                // collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) {
                    ball->Velocity.x = -multiplier * ball->Velocity.x; // reverse horizontal velocity
                    // relocate
                    float penetration = ball->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        ball->Position.x += penetration; // move ball to right
                    else
                        ball->Position.x -= penetration; // move ball to left;
                }
                else {
                    ball->Velocity.y = -multiplier * ball->Velocity.y; // reverse vertical velocity
                    // relocate
                    float penetration = ball->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        ball->Position.y -= penetration; // move ball bback up
                    else
                        ball->Position.y += penetration; // move ball back down
                }
            }
        }
    }
    // check collisions for player pad (unless stuck)
    Collision result = checkCollision(*ball, *player);
    if (!ball->Stuck && std::get<0>(result)) {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = player->Position.x + player->Size.x / 2.0f;
        float distance = (ball->Position.x + ball->Radius) - centerBoard;
        float percentage = distance / (player->Size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ball->Velocity;
        ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->Velocity = glm::normalize(ball->Velocity) * glm::length(oldVelocity); // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        // fix sticky paddle
        ball->Velocity.y = -1.0f * abs(ball->Velocity.y);
    }
    
    //Check if all blocks are destroyed
    bool won = true;
    for (GameObject& box : this->levels[this->level].Bricks) {
        if (!box.Destroyed) {
            won = false;
        }
    }
    if (won) {
        this->state = GAME_WIN;
    }
}

bool checkCollision(GameObject& one, GameObject& two) {
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

Collision checkCollision(BallObject& one, GameObject& two) {
    // get center point circle first 
    glm::vec2 center(one.Position + one.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // now that we know the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
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