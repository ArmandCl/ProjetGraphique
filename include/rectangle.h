#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>

class Rectangle : public Shape
{
public:
    Rectangle(Shader* shader_program, float width = 2.0f, float height = 1.0f, float thickness = 0.1f);
    Rectangle(Shader* shader_program, Texture* texture, float width = 2.0f, float height = 1.0f, float thickness = 0.1f);
    void setColor(glm::vec4 color) { object_color = color; }
    virtual ~Rectangle();

    void drawShadow(Shader* shader, glm::mat4 model) override;
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap) override;
    void setLight(glm::vec3 pos, glm::vec3 color) {
        lightPos = pos;
        lightColor = color;
    }

private:
    void initRectangle3D(float width, float height, float thickness);

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Texture* texture_;
    bool has_texture_;
    unsigned int vertex_count;
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    glm::vec4 object_color = glm::vec4(1.0f); // Blanc par défaut
};

#endif // RECTANGLE_H