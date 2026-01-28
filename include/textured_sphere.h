#ifndef TEXTURED_SPHERE_H
#define TEXTURED_SPHERE_H

#include "sphere.h"
#include "texture.h"

class TexturedSphere : public Sphere {
public:
    TexturedSphere(Shader *shader_program, Texture *texture);

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap) override;

private:
    GLuint loc_diffuse_map;
    Texture *texture;
};

#endif // TEXTURED_SPHERE_H
