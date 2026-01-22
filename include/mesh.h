#ifndef MESH_H
#define MESH_H
#include "shape.h"
#include "texture.h"
#include <vector>

class Mesh : public Shape {
public:
    Mesh(Shader* shader, const char* objPath, Texture* texture = nullptr);
    ~Mesh();
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    GLuint VAO, VBO, TBO, NBO;
    int vertexCount;
    Texture* texture_;
    bool has_texture_;
};
#endif