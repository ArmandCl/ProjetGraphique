#pragma once

#include "shader.h"
#include "node.h"

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Shape {
public:
    Shape(Shader *shader_program);
    bool cast_shadow = true;
    virtual void drawShadow(Shader* shader, glm::mat4 model);
    virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap);
protected:
    GLuint shader_program_;
};
