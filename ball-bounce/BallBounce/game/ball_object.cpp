#include "ball_object.h"


//Default constructor.
BallObject::BallObject() : GameObject() {
    this->radius = 12.5f;
    this->stuck = true;
}

//Constructor to set position, radius, velocity, and sprite.
BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) 
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity) {
    this->radius = radius;
    this->stuck = true;
}

//Function to move the ball, keeping it constrained within the bounds of the window. Returns new position.
glm::vec2 BallObject::move(float dt, unsigned int window_width) {
    //If not stuck:
    if (!this->stuck) {
        //Move the ball
        this->position += this->velocity * dt;
        //Check to see if the ball is outside the horizontal playfield. If so, reverse direction and reset position.
        if (this->position.x <= 0.0f) {
            this->velocity.x = -this->velocity.x;
            this->position.x = 0.0f;
        } else if (this->position.x + this->size.x >= window_width) {
            this->velocity.x = -this->velocity.x;
            this->position.x = window_width - this->size.x;
        }
        //If we are at the top of the window, reverse Y velocity.
        if (this->position.y <= 0.0f) {
            this->velocity.y = -this->velocity.y;
            this->position.y = 0.0f;
        }
    }
    return this->position;
}

//Function to reset the ball to initial stuck position.
void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {
    this->position = position;
    this->velocity = velocity;
    this->stuck = true;
}