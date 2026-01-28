#include "cylinder.h"
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

Cylinder::Cylinder(Shader* shader_program, float height, float radius, int slices, Texture* texture)
    : Shape(shader_program), height(height), radius(radius), slices(slices),
    texture_(texture), has_texture_(texture != nullptr)
{
    generateVertices();
    generateTextureCoordinates();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
        vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    if (has_texture_) {
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(glm::vec2),
            tex_coords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    num_indices = static_cast<unsigned int>(indices.size());

    glBindVertexArray(0);
}

void Cylinder::generateVertices()
{
    vertices.clear();
    indices.clear();

    for (int i = 0; i < slices; i++) {
        float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(slices);
        float x = radius * glm::cos(theta);
        float y = radius * glm::sin(theta);
        vertices.push_back(glm::vec3(x, y, 0.5f * height));
        vertices.push_back(glm::vec3(x, y, -0.5f * height));
    }

    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.5f * height));
    vertices.push_back(glm::vec3(0.0f, 0.0f, -0.5f * height));

    int center_top = vertices.size() - 2;
    int center_bottom = vertices.size() - 1;

    for (int i = 0; i < slices; i++) {
        int next = (i + 1) % slices;

        indices.push_back(2 * i);
        indices.push_back(2 * i + 1);
        indices.push_back(2 * next);

        indices.push_back(2 * i + 1);
        indices.push_back(2 * next + 1);
        indices.push_back(2 * next);
    }

    for (int i = 0; i < slices; i++) {
        int next = (i + 1) % slices;
        indices.push_back(2 * i);
        indices.push_back(2 * next);
        indices.push_back(center_top);
    }

    for (int i = 0; i < slices; i++) {
        int next = (i + 1) % slices;
        indices.push_back(2 * i + 1);
        indices.push_back(center_bottom);
        indices.push_back(2 * next + 1);
    }
}

void Cylinder::generateTextureCoordinates()
{
    tex_coords.clear();

    for (int i = 0; i <= slices; i++) {
        float u = (float)i / (float)slices;
        tex_coords.push_back(glm::vec2(u, 1.0f));
        tex_coords.push_back(glm::vec2(u, 0.0f));
    }

    for (int i = 0; i < slices; i++) {
        float angle = 2.0f * glm::pi<float>() * i / slices;
        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);
        tex_coords.push_back(glm::vec2(u, v));
        tex_coords.push_back(glm::vec2(u, v));
    }
}

void Cylinder::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap)
{
    glUseProgram(this->shader_program_);

    if (has_texture_ && texture_ != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_->getGLid());
        glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
    }

    glBindVertexArray(VAO);
    
    Shape::draw(model, view, projection, lightSpaceMatrix, shadowMap);
    
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    if (has_texture_ && texture_ != nullptr) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

Cylinder::~Cylinder()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (has_texture_) {
        glDeleteBuffers(1, &TBO);
    }
}