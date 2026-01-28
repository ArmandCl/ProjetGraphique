#include "Mesh.h"
#include "objloader.hpp"

Mesh::Mesh(Shader* shader, const char* objPath, Texture* texture, const char* objectName)
    : Shape(shader), texture_(texture), has_texture_(texture != nullptr) {

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    if (loadOBJ(objPath, vertices, uvs, normals, objectName)) {
        vertexCount = vertices.size();

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Positions
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        // UVs (Location 1)
        glGenBuffers(1, &TBO);
        glBindBuffer(GL_ARRAY_BUFFER, TBO);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);

        // Normals (Location 2 pour ton shader phong)
        glGenBuffers(1, &NBO);
        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
}

void Mesh::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);

    // --- AJOUTE CECI ICI ---
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    // -----------------------

    if (has_texture_ && texture_) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_->getGLid());
        glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
    }

    glBindVertexArray(VAO);
    Shape::draw(model, view, projection);
    glUniform3f(glGetUniformLocation(this->shader_program_, "objectColor"), 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}