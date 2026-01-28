#ifndef LIGHTING_SPHERE_H
#define LIGHTING_SPHERE_H

#include <glm/glm.hpp>
#include "shader.h"
#include "sphere.h"

class LightingSphere : public Sphere {
public:
    LightingSphere(Shader *shader_program, glm::vec3 light_position, glm::vec3 light_color, glm::vec3 object_color);

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap) override;

private:
    glm::vec3 light_position;
    glm::vec3 light_color;
    glm::vec3 object_color;

    GLint light_pos_loc;
    GLint light_color_loc;
    GLint object_color_loc;
};

#endif
