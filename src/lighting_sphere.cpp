#include "lighting_sphere.h"
#include <glm/gtc/type_ptr.hpp> 

LightingSphere::LightingSphere(Shader *shader_program, glm::vec3 light_position, glm::vec3 light_color, glm::vec3 object_color) :
    Sphere(shader_program),
    light_position(light_position),
    light_color(light_color),
    object_color(object_color)
{
    light_pos_loc = glGetUniformLocation(this->shader_program_, "lightPos");
    light_color_loc = glGetUniformLocation(this->shader_program_, "lightColor");
    object_color_loc = glGetUniformLocation(this->shader_program_, "objectColor");
}

void LightingSphere::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap)
{
    glUseProgram(this->shader_program_); 

    glUniform3fv(light_pos_loc, 1, glm::value_ptr(light_position));
    glUniform3fv(light_color_loc, 1, glm::value_ptr(light_color));
    glUniform4f(object_color_loc, object_color.r, object_color.g, object_color.b, 1.0f);

    Sphere::draw(model, view, projection, lightSpaceMatrix, shadowMap); 
}