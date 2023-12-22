#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "../graphics/texture.h"


//BallObject contains a Ball object that inherits from GameObject.
class BallObject : public GameObject {
    public:
        //Ball features.
        float radius;
        bool stuck;

        //Constructors.
        BallObject();
        BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

        //Function to move the ball, keeping it constrained within the bounds of the window. Returns new position.
        glm::vec2 move(float dt, unsigned int window_width);

        //Function to reset the ball to the passed in position and velocity
        void reset(glm::vec2 position, glm::vec2 velocity);
};

#endif