#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>

class Rectangle : public Shape
{
public:
    // Constructeur sans texture
    Rectangle(Shader* shader_program, float width = 2.0f, float height = 1.0f);

    // Constructeur avec texture (comme TexturedSphere)
    Rectangle(Shader* shader_program, Texture* texture, float width = 2.0f, float height = 1.0f);

    virtual ~Rectangle();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    void initRectangle(float width, float height);

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Texture* texture_ = nullptr;
    bool has_texture_ = false;
    unsigned int vertex_count;
};

#endif // RECTANGLE_H