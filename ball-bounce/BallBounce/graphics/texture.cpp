#include <iostream>

#include "texture.h"


//Default constructor.
Texture2D::Texture2D() {
    width = 0;
    height = 0;
    internalFormat = GL_RGB;
    imageFormat = GL_RGB;
    wrapS = GL_REPEAT;
    wrapT = GL_REPEAT;
    filterMin = GL_LINEAR;
    filterMax = GL_LINEAR;
    glGenTextures(1, &id);
}

//Function to generate texture from image data.
void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data) {
    //Update width and height.
    this->width = width;
    this->height = height;

    //Create texture.
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);

    //Set Texture wrap and filter modes.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

    //Unbind texture.
    glBindTexture(GL_TEXTURE_2D, 0);
}

//Function to bind the texture as the current active GL_TEXTURE_2D object.
void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, id);
}