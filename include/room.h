#ifndef ROOM_H
#define ROOM_H

#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <vector>

class Room : public Shape {
public:
    Room(Shader* shader_program,
        float width = 5.0f,
        float height = 3.0f,
        float depth = 5.0f);

    Room(Shader* shader_program,
        Texture* floor_texture,
        Texture* wall_texture,
        Texture* ceiling_texture,
        float width = 5.0f,
        float height = 3.0f,
        float depth = 5.0f);

    virtual ~Room();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void setLight(glm::vec3 pos, glm::vec3 color) {
        lightPos = pos;
        lightColor = color;
    }

private:
    void initRoom(float width, float height, float depth);

    struct Wall {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        Texture* texture;
        unsigned int vertex_count;
    };

    std::vector<Wall> walls;
    float width, height, depth;
    bool has_textures_;

    // Textures séparées pour chaque surface
    Texture* floor_texture_;
    Texture* wall_texture_;
    Texture* ceiling_texture_;

    glm::vec3 lightPos;
    glm::vec3 lightColor;
};

#endif // ROOM_H