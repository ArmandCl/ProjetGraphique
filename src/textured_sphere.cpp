#include "textured_sphere.h"

TexturedSphere::TexturedSphere(Shader* shader_program, Texture* texture)
    : Sphere(shader_program), texture(texture)
{
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

// src/textured_sphere.cpp

void TexturedSphere::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap) {

    glUseProgram(this->shader_program_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture->getGLid());
    glUniform1i(loc_diffuse_map, 0);

    // --- APPEL A LA CLASSE MÈRE (SPHERE) ---
    Sphere::draw(model, view, projection, lightSpaceMatrix, shadowMap);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
