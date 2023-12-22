#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


//Static storage variables.
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;


//Function to load, store, and return a shader from the given vertex, fragment, and geometry source files under the given name. Geometry data is optional.
Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name) {
    //Load the shader and store it under the passed in name.
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);

    //Return the shader.
    return shaders[name];
}


//Function to return a shader from storage.
Shader ResourceManager::getShader(std::string name) {
    return shaders[name];
}

//Function to load, store, and return a texture from the given file under the given name.
Texture2D ResourceManager::loadTexture(const char* file, bool alpha, std::string name) {
    //Load the texture and store it under the passed in name.
    textures[name] = loadTextureFromFile(file, alpha);

    //Return the texture.
    return textures[name];
}

//Function to return a texture from storage.
Texture2D ResourceManager::getTexture(std::string name) {
    return textures[name];
}

//Function to clear all storage.
void ResourceManager::clear() {
    //Iterate through the shaders and delete them.
    for (auto iter : shaders)
        glDeleteProgram(iter.second.id);

    //Iterate through the textures and delete them.
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.id);
}


//Function to load and return a shader from a file.
Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
    //Retrieve the vertex, fragment, and geometry source code from their file paths.
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try {
        //Open the vertex and shader files.
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);

        //Create stream storage.
        std::stringstream vShaderStream, fShaderStream;

        //Store file's buffer contents into streams.
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        //Close file handlers.
        vertexShaderFile.close();
        fragmentShaderFile.close();

        //Convert streams into strings.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        //If geometry shader path is not null, also load a geometry shader.
        if (gShaderFile != nullptr) {
            //Open the geometry file.
            std::ifstream geometryShaderFile(gShaderFile);

            //Create stream storage.
            std::stringstream gShaderStream;

            //Store file's buffer contents into stream.
            gShaderStream << geometryShaderFile.rdbuf();

            //Close file handler.
            geometryShaderFile.close();

            //Convert stream into string.
            geometryCode = gShaderStream.str();
        }
    } catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    //Compile codes into char* pointers.
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    //Create shader object from source code.
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);

    //Return the created shader.
    return shader;
}

//Function to load and return a texture from a file.
Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
    // create texture object
    Texture2D texture;

    //If the image has an alpha value, update the texture formats.
    if (alpha) {
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }

    //Load the image.
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    //Generate the texture from the image data.
    texture.generate(width, height, data);

    //Free the image data.
    stbi_image_free(data);

    //Return the created texture.
    return texture;
}