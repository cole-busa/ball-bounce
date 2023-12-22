#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


//Shader contains the framework for a shader.
class Shader {
    public:
        //Each shader has a unique id.
        unsigned int id;

        //Empty constructor.
        Shader() {}

        //Function to use and return the current shader.
        Shader& use();

        //Function to compile a shader from given vertex, fragment, and geometry source files.
        void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

        //Helper functions to set OpenGL shader properties.
        void setFloat(const char* name, float value, bool useShader = false);
        void setInteger(const char* name, int value, bool useShader = false);
        void setVector2f(const char* name, float x, float y, bool useShader = false);
        void setVector2f(const char* name, const glm::vec2& value, bool useShader = false);
        void setVector3f(const char* name, float x, float y, float z, bool useShader = false);
        void setVector3f(const char* name, const glm::vec3& value, bool useShader = false);
        void setVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
        void setVector4f(const char* name, const glm::vec4& value, bool useShader = false);
        void setMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

    private:
        //Function to ensure shaders of an object are working properly.
        void checkCompileErrors(unsigned int object, std::string type);
};

#endif