#include "game_object.h"


GameObject::GameObject() {
    this->Position = glm::vec2(0.0f, 0.0f);
    this->Size = glm::vec2(1.0f, 1.0f);
    this->Velocity = glm::vec2(0.0f);
    this->Color = glm::vec3(1.0f);
    this->Rotation = 0.0f;
    this->Sprite;
    this->IsSolid = false;
    this->Destroyed = false;
    this->IsBouncy = false;
    this->IsEnlarging = false;
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) { 
    this->Position = pos;
    this->Size = size;
    this->Velocity = velocity;
    this->Color = color;
    this->Rotation = 0.0f;
    this->Sprite = sprite; 
    this->IsSolid = false;
    this->Destroyed = false;
    this->IsBouncy = false;
    this->IsEnlarging = false;
}

void GameObject::Draw(SpriteRenderer& renderer) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}