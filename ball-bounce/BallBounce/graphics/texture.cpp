#include <iostream>

#include "texture.h"


Texture2D::Texture2D() {
    this->width = 0;
    this->height = 0;
    this->internalFormat = GL_RGB;
    this->imageFormat = GL_RGB;
    this->wrapS = GL_REPEAT;
    this->wrapT = GL_REPEAT;
    this->filterMin = GL_LINEAR;
    this->filterMax = GL_LINEAR;
    glGenTextures(1, &this->id);
}

void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data) {
    this->width = width;
    this->height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->id);
}