#include "room.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

Room::Room(Shader* shader_program, float width, float height, float depth)
    : Shape(shader_program),
    width(width), height(height), depth(depth),
    floor_texture_(nullptr), wall_texture_(nullptr), ceiling_texture_(nullptr),
    has_textures_(false)
{
    initRoom(width, height, depth, 0.15f);
}

Room::Room(Shader* shader_program,
    Texture* floor_texture,
    Texture* wall_texture,
    Texture* ceiling_texture,
    float width, float height, float depth, float thickness)
    : Shape(shader_program),
    width(width), height(height), depth(depth),
    floor_texture_(floor_texture),
    wall_texture_(wall_texture),
    ceiling_texture_(ceiling_texture),
    has_textures_(true)
{
    initRoom(width, height, depth, thickness);
}

Room::~Room() {
    for (auto& wall : walls) {
        glDeleteVertexArrays(1, &wall.VAO);
        glDeleteBuffers(1, &wall.VBO);
        glDeleteBuffers(1, &wall.EBO);
    }
}

void Room::initRoom(float width, float height, float depth, float thickness) {
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    float hd = depth * 0.5f;

    // Le petit décalage pour éviter le glitch de texture (Z-fighting)
    float eps = 0.001f; 

    // On définit le bas du mur
    float y_limit_bot = -hh - thickness;

    auto createPart = [&](const std::vector<float>& v, Texture* tex) {
        Wall wall; 
        wall.texture = tex;
        wall.vertex_count = 6;
        std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

        glGenVertexArrays(1, &wall.VAO);
        glGenBuffers(1, &wall.VBO);
        glGenBuffers(1, &wall.EBO);

        glBindVertexArray(wall.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, wall.VBO);
        glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wall.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position (Location 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        // Texture (Location 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // Normale (Location 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

        walls.push_back(wall);
    };

    // --- COORDONNÉES DE TEXTURE (TILING) ---
    float uW = width * 0.5f; 
    float vD = depth * 0.5f; 
    float vH = height * 0.5f; 

    // --- 1. SOL ---
    float y_floor_top = -hh;
    float y_floor_bot = -hh - thickness;
    
    // Sol principal
    createPart({ 
        -hw, y_floor_top,  hd,  0.0f, 0.0f,  0,1,0, 
         hw, y_floor_top,  hd,  uW,   0.0f,  0,1,0, 
         hw, y_floor_top, -hd,  uW,   vD,    0,1,0, 
        -hw, y_floor_top, -hd,  0.0f, vD,    0,1,0 
    }, floor_texture_);
    
    // Sol dessous/bords (optionnel)
    createPart({ -hw, y_floor_bot, -hd, 0,0, 0,-1,0,  hw, y_floor_bot, -hd, 1,0, 0,-1,0,  hw, y_floor_bot, hd, 1,1, 0,-1,0,  -hw, y_floor_bot, hd, 0,1, 0,-1,0 }, floor_texture_);
    createPart({ -hw, y_floor_bot, hd, 0,0, 0,0,1,  hw, y_floor_bot, hd, 1,0, 0,0,1,  hw, y_floor_top, hd, 1,1, 0,0,1,  -hw, y_floor_top, hd, 0,1, 0,0,1 }, floor_texture_); 
    createPart({ -hw, y_floor_bot, -hd, 0,0, 0,0,-1, -hw, y_floor_top, -hd, 0,1, 0,0,-1, hw, y_floor_top, -hd, 1,1, 0,0,-1,  hw, y_floor_bot, -hd, 1,0, 0,0,-1 }, floor_texture_); 
    createPart({ -hw, y_floor_bot, -hd, 0,0, -1,0,0, -hw, y_floor_bot, hd, 1,0, -1,0,0,  -hw, y_floor_top, hd, 1,1, -1,0,0,  -hw, y_floor_top, -hd, 0,1, -1,0,0 }, floor_texture_);

    // --- 2. MUR GAUCHE ---
    float x_in = -hw - eps;
    float x_out = -hw - thickness - eps;

    // Intérieur
    createPart({ 
        x_in, y_limit_bot,  hd,  0.0f, 0.0f,  1,0,0, 
        x_in, y_limit_bot, -hd,  vD,   0.0f,  1,0,0, 
        x_in, hh,          -hd,  vD,   vH,    1,0,0, 
        x_in, hh,           hd,  0.0f, vH,    1,0,0 
    }, wall_texture_);

    // Extérieur
    createPart({ x_out, y_limit_bot, -hd, 0,0, -1,0,0, x_out, y_limit_bot, hd, 1,0, -1,0,0,  x_out, hh, hd, 1,1, -1,0,0,  x_out, hh, -hd, 0,1, -1,0,0 }, wall_texture_);
    createPart({ x_out, hh, hd, 0,0, 0,1,0,   x_in, hh, hd, 1,0, 0,1,0,    x_in, hh, -hd, 1,1, 0,1,0,  x_out, hh, -hd, 0,1, 0,1,0 }, wall_texture_); 
    createPart({ x_out, y_limit_bot, -hd, 0,0, 0,-1,0, x_in, y_limit_bot, -hd, 1,0, 0,-1,0,  x_in, y_limit_bot, hd, 1,1, 0,-1,0,  x_out, y_limit_bot, hd, 0,1, 0,-1,0 }, wall_texture_); 
    createPart({ x_out, y_limit_bot, -hd, 0,0, 0,0,-1, x_out, hh, -hd, 0,1, 0,0,-1,  x_in, hh, -hd, 1,1, 0,0,-1,  x_in, y_limit_bot, -hd, 1,0, 0,0,-1 }, wall_texture_);

    // --- 3. MUR ARRIÈRE ---
    float z_in = -hd - eps;
    float z_out = -hd - thickness - eps;
    float win_w = width * 0.35f; float win_h = height * 0.45f;
    float x_off = width * 0.2f;
    float xL = x_off - (win_w * 0.5f); float xR = x_off + (win_w * 0.5f);
    float yB = -(win_h * 0.5f); float yT = (win_h * 0.5f);

    // Faces avant (Intérieur)
    createPart({ -hw, y_limit_bot, z_in, 0,0, 0,0,1, hw, y_limit_bot, z_in, uW,0, 0,0,1, hw, yB, z_in, uW,0.3f, 0,0,1, -hw, yB, z_in, 0,0.3f, 0,0,1 }, wall_texture_);
    createPart({ -hw, yT, z_in, 0,0.7f, 0,0,1,  hw, yT, z_in, uW,0.7f, 0,0,1,  hw, hh, z_in, uW,vH, 0,0,1, -hw, hh, z_in, 0,vH, 0,0,1 }, wall_texture_);
    createPart({ -hw, yB, z_in, 0,0.3f, 0,0,1,  xL, yB, z_in, 0.3f,0.3f, 0,0,1,  xL, yT, z_in, 0.3f,0.7f, 0,0,1, -hw, yT, z_in, 0,0.7f, 0,0,1 }, wall_texture_);
    createPart({ xR, yB, z_in, 0.7f,0.3f, 0,0,1,   hw, yB, z_in, uW,0.3f, 0,0,1,  hw, yT, z_in, uW,0.7f, 0,0,1, xR, yT, z_in, 0.7f,0.7f, 0,0,1 }, wall_texture_);

    // Faces arrières (Extérieur)
    createPart({ hw, y_limit_bot, z_out, 0,0, 0,0,-1, -hw, y_limit_bot, z_out, 1,0, 0,0,-1, -hw, yB, z_out, 1,1, 0,0,-1, hw, yB, z_out, 0,1, 0,0,-1 }, wall_texture_);
    createPart({ hw, yT, z_out, 0,0, 0,0,-1,  -hw, yT, z_out, 1,0, 0,0,-1,  -hw, hh, z_out, 1,1, 0,0,-1, hw, hh, z_out, 0,1, 0,0,-1 }, wall_texture_);
    createPart({ xL, yB, z_out, 0,0, 0,0,-1,  -hw, yB, z_out, 1,0, 0,0,-1,  -hw, yT, z_out, 1,1, 0,0,-1, xL, yT, z_out, 0,1, 0,0,-1 }, wall_texture_);
    createPart({ hw, yB, z_out, 0,0, 0,0,-1,   xR, yB, z_out, 1,0, 0,0,-1,   xR, yT, z_out, 1,1, 0,0,-1, hw, yT, z_out, 0,1, 0,0,-1 }, wall_texture_);

    // Tunnel fenêtre
    createPart({ xL, yB, z_in, 0,0, 0,1,0, xR, yB, z_in, 1,0, 0,1,0, xR, yB, z_out, 1,1, 0,1,0, xL, yB, z_out, 0,1, 0,1,0 }, wall_texture_);
    createPart({ xL, yT, z_out, 0,0, 0,-1,0, xR, yT, z_out, 1,0, 0,-1,0, xR, yT, z_in, 1,1, 0,-1,0, xL, yT, z_in, 0,1, 0,-1,0 }, wall_texture_);
    createPart({ xL, yB, z_out, 0,0, 1,0,0, xL, yT, z_out, 1,0, 1,0,0, xL, yT, z_in, 1,1, 1,0,0, xL, yB, z_in, 0,1, 1,0,0 }, wall_texture_);
    createPart({ xR, yB, z_in, 0,0, -1,0,0, xR, yT, z_in, 1,0, -1,0,0, xR, yT, z_out, 1,1, -1,0,0, xR, yB, z_out, 0,1, -1,0,0 }, wall_texture_);

    // Tranches
    createPart({ -hw, hh, z_in, 0,0, 0,1,0,  hw, hh, z_in, 1,0, 0,1,0,  hw, hh, z_out, 1,1, 0,1,0,  -hw, hh, z_out, 0,1, 0,1,0 }, wall_texture_); 
    createPart({ -hw, y_limit_bot, z_out, 0,0, 0,-1,0, hw, y_limit_bot, z_out, 1,0, 0,-1,0, hw, y_limit_bot, z_in, 1,1, 0,-1,0,  -hw, y_limit_bot, z_in, 0,1, 0,-1,0 }, wall_texture_); 
    createPart({ -hw, y_limit_bot, z_in, 0,0, -1,0,0, -hw, y_limit_bot, z_out, 1,0, -1,0,0, -hw, hh, z_out, 1,1, -1,0,0, -hw, hh, z_in, 0,1, -1,0,0 }, wall_texture_); 
    createPart({ hw, y_limit_bot, z_out, 0,0, 1,0,0, hw, y_limit_bot, z_in, 1,0, 1,0,0, hw, hh, z_in, 1,1, 1,0,0, hw, hh, z_out, 0,1, 1,0,0 }, wall_texture_); 
}

// === OMBRES (PASSE 1) ===
void Room::drawShadow(Shader* shader, glm::mat4 model) {
    shader->setMat4("model", model);
    for (const auto& wall : walls) {
        glBindVertexArray(wall.VAO);
        glDrawElements(GL_TRIANGLES, wall.vertex_count, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

// === RENDU FINAL (PASSE 2) ===
void Room::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpaceMatrix, GLuint shadowMap) {
    glUseProgram(this->shader_program_);
    
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(this->shader_program_, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    
    for (auto& wall : walls) {
        if (has_textures_ && wall.texture != nullptr) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wall.texture->getGLid());
            glUniform1i(glGetUniformLocation(this->shader_program_, "diffuse_map"), 0);
        }

        // Appel parent avec 5 arguments
        Shape::draw(model, view, projection, lightSpaceMatrix, shadowMap);

        glBindVertexArray(wall.VAO);
        glDrawElements(GL_TRIANGLES, wall.vertex_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}