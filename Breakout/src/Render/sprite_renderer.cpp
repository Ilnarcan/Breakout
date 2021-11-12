#include "sprite_renderer.h"


SpriteRenderer::SpriteRenderer(const Shader& shader)
{
    this->shader = shader;
    this->init_render_data();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::draw_sprite(const Texture2D* texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    // Preparing transformations
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); 

    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader.set_matrix4("model", model);

    // Render a textured rectangle
    this->shader.set_vector_3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::init_render_data()
{
	// Configuring VAO / VBO

    unsigned int VBO;
    const float vertices[] = {
        0.0f, 1.0f, 
        1.0f, 0.0f, 
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f, 
        1.0f, 0.0f
    };

     const float text_coord[] = {
         0.0f, 1.0f,
         1.0f, 0.0f,
         0.0f, 0.0f,

         0.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, 0.0f
     };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(text_coord), text_coord, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}