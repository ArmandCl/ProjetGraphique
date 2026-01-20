#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>

class Rectangle : public Shape
{
public:
    // Rectangle avec épaisseur (3D)
    Rectangle(Shader* shader_program, float width = 2.0f, float height = 1.0f, float thickness = 0.1f);
    Rectangle(Shader* shader_program, Texture* texture, float width = 2.0f, float height = 1.0f, float thickness = 0.1f);

    virtual ~Rectangle();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    void initRectangle3D(float width, float height, float thickness);

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Texture* texture_;
    bool has_texture_;
    unsigned int vertex_count;
};

#endif // RECTANGLE_H