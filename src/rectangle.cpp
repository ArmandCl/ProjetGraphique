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

    // Format: PosX, PosY, PosZ, TexU, TexV, NormX, NormY, NormZ
    std::vector<float> vertices = {
        // Face avant (Z+)
        -half_width, -half_height,  half_thickness,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         half_width, -half_height,  half_thickness,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         half_width,  half_height,  half_thickness,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -half_width,  half_height,  half_thickness,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        // Face arri�re (Z-)
        -half_width, -half_height, -half_thickness,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
        -half_width,  half_height, -half_thickness,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         half_width,  half_height, -half_thickness,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         half_width, -half_height, -half_thickness,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         // Face gauche (X-)
         -half_width, -half_height, -half_thickness,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
         -half_width, -half_height,  half_thickness,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
         -half_width,  half_height,  half_thickness,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
         -half_width,  half_height, -half_thickness,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
         // Face droite (X+)
          half_width, -half_height, -half_thickness,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
          half_width,  half_height, -half_thickness,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
          half_width,  half_height,  half_thickness,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
          half_width, -half_height,  half_thickness,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
          // Face haut (Y+)
          -half_width,  half_height, -half_thickness,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
          -half_width,  half_height,  half_thickness,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
           half_width,  half_height,  half_thickness,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
           half_width,  half_height, -half_thickness,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
           // Face bas (Y-)
           -half_width, -half_height, -half_thickness,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
            half_width, -half_height, -half_thickness,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
            half_width, -half_height,  half_thickness,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
           -half_width, -half_height,  half_thickness,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,       // Avant
        4, 5, 6, 6, 7, 4,       // Arri�re
        8, 9, 10, 10, 11, 8,    // Gauche
        12, 13, 14, 14, 15, 12, // Droite
        16, 17, 18, 18, 19, 16, // Haut
        20, 21, 22, 22, 23, 20  // Bas
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture (Location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normale (Location 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    this->vertex_count = static_cast<GLuint>(indices.size());
    glBindVertexArray(0);
}

Rectangle::~Rectangle()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// src/rectangle.cpp

void Rectangle::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap)
{
    // 1. DETERMINER SI TRANSPARENT
    bool isTransparent = (object_color.a < 1.0f);

    // 2. ACTIVER LE SHADER D'ABORD
    glUseProgram(this->shader_program_);

    // --- 3. ENVOI DES MATRICES ET OMBRES VIA SHAPE::DRAW ---
    // (Ceci remplace tes 3 anciens appels glUniformMatrix4fv)
    Shape::draw(model, view, projection, lightSpaceMatrix, shadowMap);

    // 4. ENVOYER LA COULEUR ET LA LUMIERE (Spécifique au rectangle)
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "objectColor");
    glUniform4f(colorLoc, object_color.r, object_color.g, object_color.b, object_color.a);
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightColor"), lightColor.x, lightColor.y, lightColor.z);

    // 5. GERER LA TEXTURE
    if (has_texture_ && texture_ != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_->getGLid());
        glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
    }

    // 6. GESTION DE LA TRANSPARENCE
    if(isTransparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    // 7. DESSINER
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if(isTransparent) glDepthMask(GL_TRUE);
}