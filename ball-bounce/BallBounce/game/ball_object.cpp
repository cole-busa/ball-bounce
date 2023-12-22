#include "ball_object.h"


//Default constructor.
BallObject::BallObject() : GameObject() {
    radius = 12.5f;
    stuck = true;
}

//Constructor to set position, radius, velocity, and sprite.
BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) 
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity) {
    this->radius = radius;
    stuck = true;
}

//Function to move the ball, keeping it constrained within the bounds of the window. Returns new position.
glm::vec2 BallObject::move(float dt, unsigned int window_width) {
    //If not stuck:
    if (!stuck) {
        //Move the ball
        position += velocity * dt;

        //Check to see if the ball is outside the horizontal playfield. If so, reverse direction and reset position.
        if (position.x <= 0.0f) {
            velocity.x = -velocity.x;
            position.x = 0.0f;
        } else if (position.x + size.x >= window_width) {
            velocity.x = -velocity.x;
            position.x = window_width - size.x;
        }

        //If we are at the top of the window, reverse Y velocity.
        if (position.y <= 0.0f) {
            velocity.y = -velocity.y;
            position.y = 0.0f;
        }
    }

    //Return the updated position.
    return position;
}

//Function to reset the ball to the passed in position and velocity.
void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {
    this->position = position;
    this->velocity = velocity;
    stuck = true;
}