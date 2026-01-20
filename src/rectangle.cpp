#include "rectangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

Rectangle::Rectangle(Shader* shader_program, float width, float height, float thickness)
    : Shape(shader_program), texture_(nullptr), has_texture_(false)
{
    initRectangle3D(width, height, thickness);
}

Rectangle::Rectangle(Shader* shader_program, Texture* texture, float width, float height, float thickness)
    : Shape(shader_program), texture_(texture), has_texture_(true)
{
    initRectangle3D(width, height, thickness);
}

void Rectangle::initRectangle3D(float width, float height, float thickness)
{
    float half_width = width * 0.5f;
    float half_height = height * 0.5f;
    float half_thickness = thickness * 0.5f;

    // Vertices d'un parallélépipède rectangle
    // Format: position.x, position.y, position.z, texCoord.x, texCoord.y
    // On créé les 8 sommets d'un cube
    std::vector<float> vertices = {
        // Face avant (Z positif)
        -half_width, -half_height,  half_thickness,   0.0f, 0.0f,
         half_width, -half_height,  half_thickness,   1.0f, 0.0f,
         half_width,  half_height,  half_thickness,   1.0f, 1.0f,
        -half_width,  half_height,  half_thickness,   0.0f, 1.0f,

        // Face arrière (Z négatif)
        -half_width, -half_height, -half_thickness,   1.0f, 0.0f,
         half_width, -half_height, -half_thickness,   0.0f, 0.0f,
         half_width,  half_height, -half_thickness,   0.0f, 1.0f,
        -half_width,  half_height, -half_thickness,   1.0f, 1.0f
    };

    // Indices pour 6 faces (2 triangles par face)
    std::vector<unsigned int> indices = {
        // Face avant
        0, 1, 2, 2, 3, 0,
        // Face arrière
        4, 5, 6, 6, 7, 4,
        // Face droite
        1, 5, 6, 6, 2, 1,
        // Face gauche
        0, 4, 7, 7, 3, 0,
        // Face haut
        3, 2, 6, 6, 7, 3,
        // Face bas
        0, 1, 5, 5, 4, 0
    };

    vertex_count = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

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

    if (has_texture_ && texture_ != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_->getGLid());
        glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
    }

    glBindVertexArray(VAO);
    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (has_texture_ && texture_ != nullptr)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}