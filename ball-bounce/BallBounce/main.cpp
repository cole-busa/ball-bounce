#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "graphics/resource_manager.h"

#include <iostream>


//GLFW helper functions.
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Width and height  of the screen.
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

//Game variable.
Game ballBounce(SCREEN_WIDTH, SCREEN_HEIGHT);

//Main function to link the game to OpenGL.
int main(int argc, char* argv[]) {
    //Initialize GLFW, the OpenGL library.
    glfwInit();

    //Set window properties.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    //Create the GLFW window and pass it in.
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ball Bounce", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Load all OpenGL function pointers with GLAD. Report an error if unsuccessful.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Pass in helper functions.
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //Apply settings to OpenGL.
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Intialize the game.
    ballBounce.init();

    //Time variables.
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //Play the game while the window is open.
    while (!glfwWindowShouldClose(window)) {
        //Calculate the change in time.
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Check for events.
        glfwPollEvents();

        //Process input within the game.
        ballBounce.processInput(deltaTime);

        //Update the game.
        ballBounce.update(deltaTime);

        //Reset color parameters.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Render the game since the last frame.
        ballBounce.render();

        //Swap the window buffer.
        glfwSwapBuffers(window);
    }

    //Clear all stored resources like textures and sprites.
    ResourceManager::clear();

    //Terminate GLFW and return.
    glfwTerminate();
    return 0;
}


//Helper function to track user key input.
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    //If the user presses escape we want to close the application.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //In any case, pass the key information into the game.
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            ballBounce.keys[key] = true;
        else if (action == GLFW_RELEASE)
            ballBounce.keys[key] = false;
    }
}

//Helper function to ensure the GLFW viewport is the same size as the window.
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}