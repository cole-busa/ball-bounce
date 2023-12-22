#include "sprite_renderer.h"


//Constructor that intializes a passed in shader.
SpriteRenderer::SpriteRenderer(Shader shader) {
    this->shader = shader;
    initRenderData();
}

//Destructor.
SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

//Function to render a passed in sprite.
void SpriteRenderer::drawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    //Use the shader and initialize the model.
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);

    //First, translate the model.
    model = glm::translate(model, glm::vec3(position, 0.0f));  

    //Then, move the origin of rotation to the center of quad.
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

    //Next, rotate the model.
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));

    //Then, move the origin back.
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); 

    //Finally, rescale the model.
    model = glm::scale(model, glm::vec3(size, 1.0f));

    //Store the model in the shader.
    shader.setMatrix4("model", model);

    //Set the passed in color.
    shader.setVector3f("spriteColor", color);

    //Set the texture as active.
    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    //Render the textured quad.
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


//Function to initialize and configure the quad's VAO and VBO attributes.
void SpriteRenderer::initRenderData() {
    //VBO storage.
    unsigned int VBO;

    //Position and texture values.
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    //Initialize VAO and VBO.
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    //Set the VBO buffer data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Configure the VAO.
    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}