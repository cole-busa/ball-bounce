#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"


//ResourceManager is a framework for loading and storing shaders and textures.
class ResourceManager {
    public:
        //Storage for shaders and textures.
        static std::map<std::string, Shader> shaders;
        static std::map<std::string, Texture2D> textures;

        //Function to load, store, and return a shader from the given vertex, fragment, and geometry source files under the given name. Geometry data is optional.
        static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);

        //Function to return a shader from storage.
        static Shader getShader(std::string name);

        //Function to load, store, and return a texture from the given file under the given name.
        static Texture2D loadTexture(const char* file, bool alpha, std::string name);

        //Function to return a texture from storage.
        static Texture2D getTexture(std::string name);

        //Function to clear all storage.
        static void clear();

    private:
        //Private constructor because there should not be a resource manager object.
        ResourceManager() {}

        //Function to load and return a shader from a file.
        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

        //Function to load and return a texture from a file.
        static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

#endif