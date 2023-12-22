#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../graphics/texture.h"
#include "../graphics/sprite_renderer.h"


//GameObject contains a the framework for base object in Ball Bounce.
//Each GameObject will have a position, size, velocity, color, and rotation.
//It will also have booleans depicting whether or not it is destroyed,
//bouncy, enlarging, or cloning.
class GameObject {
    public:
        //Object properties.
        glm::vec2 position, size, velocity;
        glm::vec3 color;
        float rotation;

        //Booleans denoting special properties of the object.
        bool isDestroyed, isBouncy, isEnlarging, isCloning;

        //Texture of the object.
        Texture2D sprite;

        //Constructors.
        GameObject();
        GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f), std::string type = "none");
        
        //Function to draw the object's sprite.
        void draw(SpriteRenderer& renderer);
};

#endif