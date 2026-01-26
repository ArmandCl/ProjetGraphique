#include "room.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

Room::Room(Shader* shader_program, float width, float height, float depth)
    : Shape(shader_program),
    width(width), height(height), depth(depth),
    floor_texture_(nullptr), wall_texture_(nullptr), ceiling_texture_(nullptr),
    has_textures_(false)
{
    initRoom(width, height, depth);
}

Room::Room(Shader* shader_program,
    Texture* floor_texture,
    Texture* wall_texture,
    Texture* ceiling_texture,
    float width, float height, float depth)
    : Shape(shader_program),
    width(width), height(height), depth(depth),
    floor_texture_(floor_texture),
    wall_texture_(wall_texture),
    ceiling_texture_(ceiling_texture),
    has_textures_(true)
{
    initRoom(width, height, depth);
}

void Room::initRoom(float width, float height, float depth) {
    float half_width = width * 0.5f;
    float half_height = height * 0.5f;
    float half_depth = depth * 0.5f;

    // Créer les 6 parois (sol, plafond, 4 murs)

    // 1. SOL (Y = -half_height)
    {
        Wall floor;
        std::vector<float> vertices = {
            -half_width, -half_height, -half_depth,   0.0f, 0.0f,
             half_width, -half_height, -half_depth,   1.0f, 0.0f,
             half_width, -half_height,  half_depth,   1.0f, 1.0f,
            -half_width, -half_height,  half_depth,   0.0f, 1.0f
        };

        std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
        floor.vertex_count = static_cast<unsigned int>(indices.size());
        floor.texture = floor_texture_;

        glGenVertexArrays(1, &floor.VAO);
        glGenBuffers(1, &floor.VBO);
        glGenBuffers(1, &floor.EBO);

        glBindVertexArray(floor.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, floor.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
        walls.push_back(floor);
    }
    // 3. MUR ARRIÈRE (Z = -half_depth)
    {
        Wall back_wall;
        std::vector<float> vertices = {
            -half_width, -half_height, -half_depth,   0.0f, 0.0f,
             half_width, -half_height, -half_depth,   1.0f, 0.0f,
             half_width,  half_height, -half_depth,   1.0f, 1.0f,
            -half_width,  half_height, -half_depth,   0.0f, 1.0f
        };

        std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
        back_wall.vertex_count = static_cast<unsigned int>(indices.size());
        back_wall.texture = wall_texture_;

        glGenVertexArrays(1, &back_wall.VAO);
        glGenBuffers(1, &back_wall.VBO);
        glGenBuffers(1, &back_wall.EBO);

        glBindVertexArray(back_wall.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, back_wall.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, back_wall.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
        walls.push_back(back_wall);
    }

    // 5. MUR GAUCHE (X = -half_width)
    {
        Wall left_wall;
        std::vector<float> vertices = {
            -half_width, -half_height, -half_depth,   0.0f, 0.0f,
            -half_width, -half_height,  half_depth,   1.0f, 0.0f,
            -half_width,  half_height,  half_depth,   1.0f, 1.0f,
            -half_width,  half_height, -half_depth,   0.0f, 1.0f
        };

        std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
        left_wall.vertex_count = static_cast<unsigned int>(indices.size());
        left_wall.texture = wall_texture_;

        glGenVertexArrays(1, &left_wall.VAO);
        glGenBuffers(1, &left_wall.VBO);
        glGenBuffers(1, &left_wall.EBO);

        glBindVertexArray(left_wall.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, left_wall.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, left_wall.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
        walls.push_back(left_wall);
    }

    

    std::cout << "Room created: " << width << "x" << height << "x" << depth << std::endl;
}

void Room::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    // Envoi de la lumière
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    for (auto& wall : walls) {
        if (has_textures_ && wall.texture != nullptr) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wall.texture->getGLid());
            glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
        }

        glBindVertexArray(wall.VAO);
        Shape::draw(model, view, projection);
        glDrawElements(GL_TRIANGLES, wall.vertex_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        if (has_textures_ && wall.texture != nullptr) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

Room::~Room() {
    for (auto& wall : walls) {
        glDeleteVertexArrays(1, &wall.VAO);
        glDeleteBuffers(1, &wall.VBO);
        glDeleteBuffers(1, &wall.EBO);
    }
}