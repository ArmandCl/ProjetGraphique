#include "rectangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

Rectangle::Rectangle(Shader* shader_program, float width, float height)
    : Shape(shader_program), texture_(nullptr), has_texture_(false)
{
    initRectangle(width, height);
}

Rectangle::Rectangle(Shader* shader_program, Texture* texture, float width, float height)
    : Shape(shader_program), texture_(texture), has_texture_(true)
{
    initRectangle(width, height);
}

void Rectangle::initRectangle(float width, float height)
{
    float half_width = width * 0.5f;
    float half_height = height * 0.5f;

    // Vertices avec positions ET coordonnées de texture
    // Format: position.x, position.y, position.z, texCoord.x, texCoord.y
    std::vector<float> vertices = {
        // Positions          // Texture Coords
        -half_width, -half_height, 0.0f,   0.0f, 0.0f,
         half_width, -half_height, 0.0f,   1.0f, 0.0f,
         half_width,  half_height, 0.0f,   1.0f, 1.0f,
        -half_width,  half_height, 0.0f,   0.0f, 1.0f
    };

    // Indices pour former 2 triangles
    std::vector<unsigned int> indices = {
        0, 1, 2,   // Premier triangle
        2, 3, 0    // Second triangle
    };

    vertex_count = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // Texture coordinate attribute (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

Rectangle::~Rectangle()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Rectangle::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUseProgram(this->shader_program_);

    // Activer la texture si elle existe (comme dans textured_sphere.cpp)
    if (has_texture_ && texture_ != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_->getGLid());

        // Uniform "diffuse_map" comme dans votre texture.frag
        glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
    }

    glBindVertexArray(VAO);
    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Désactiver la texture si nécessaire
    if (has_texture_ && texture_ != nullptr)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}