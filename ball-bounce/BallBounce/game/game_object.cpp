#include "game_object.h"


GameObject::GameObject() {
    this->position = glm::vec2(0.0f, 0.0f);
    this->size = glm::vec2(1.0f, 1.0f);
    this->velocity = glm::vec2(0.0f);
    this->color = glm::vec3(1.0f);
    this->rotation = 0.0f;
    this->sprite;
    this->isSolid = false;
    this->destroyed = false;
    this->isBouncy = false;
    this->isEnlarging = false;
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) { 
    this->position = pos;
    this->size = size;
    this->velocity = velocity;
    this->color = color;
    this->rotation = 0.0f;
    this->sprite = sprite; 
    this->isSolid = false;
    this->destroyed = false;
    this->isBouncy = false;
    this->isEnlarging = false;
}

void GameObject::draw(SpriteRenderer& renderer) {
    renderer.drawSprite(this->sprite, this->position, this->size, this->rotation, this->color);
}