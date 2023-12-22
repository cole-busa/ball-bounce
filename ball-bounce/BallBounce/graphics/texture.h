#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

//Texture2D contains the means to store and configure a texture.
class Texture2D {
    public:
        //Every texture has a unique id.
        unsigned int id;

        //Texture image dimensions in pixels.
        unsigned int width, height;

        //Texture format.
        unsigned int internalFormat;

        //Loaded image format.
        unsigned int imageFormat;

        //Texture configuration:
        //Wrapping mode on S axis.
        unsigned int wrapS;

        //Wrapping mode on T axis
        unsigned int wrapT; 

        //Filtering mode if texture pixels < screen pixels.
        unsigned int filterMin; 
        //Filtering mode if texture pixels > screen pixels.
        unsigned int filterMax;

        //Default constructor.
        Texture2D();

        //Function to generate texture from image data.
        void generate(unsigned int width, unsigned int height, unsigned char* data);

        //Function to bind the texture as the current active GL_TEXTURE_2D object.
        void bind() const;
};

#endif