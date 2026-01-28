
#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "texture.h"

class Cylinder : public Shape
{
public:
    Cylinder(Shader* shader_program, float height = 1.0f,
        float radius = 0.5f, int slices = 32,
        Texture* texture = nullptr);

    virtual ~Cylinder();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap) override;

    void setTexture(Texture* texture) { texture_ = texture; has_texture_ = true; }

private:
    void generateVertices();
    void generateTextureCoordinates();

    float height;
    float radius;
    int slices;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int TBO;  
    unsigned int num_indices;

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> tex_coords;

    Texture* texture_;
    bool has_texture_;
};

#endif // CYLINDER_H