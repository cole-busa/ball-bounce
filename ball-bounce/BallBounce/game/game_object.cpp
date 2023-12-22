#include "game_object.h"


//Default constructor.
GameObject::GameObject() {
    position = glm::vec2(0.0f, 0.0f);
    size = glm::vec2(1.0f, 1.0f);
    velocity = glm::vec2(0.0f);
    color = glm::vec3(1.0f);
    rotation = 0.0f;
    sprite;
    isDestroyed = false;
    isBouncy = false;
    isEnlarging = false;
    isCloning = false;
}

//Constructor passing in position, size, sprite, color, velocity, and type of the GameObject.
GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, std::string type) { 
    this->position = position;
    this->size = size;
    this->velocity = velocity;
    this->color = color;
    this->sprite = sprite; 
    rotation = 0.0f;
    isDestroyed = false;
    isBouncy = type == "bouncy";
    isEnlarging = type == "enlarging";
    isCloning = type == "cloning";
}

//Function to draw the object's sprite.
void GameObject::draw(SpriteRenderer& renderer) {
    renderer.drawSprite(sprite, position, size, rotation, color);
}