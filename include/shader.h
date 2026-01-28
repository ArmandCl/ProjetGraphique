#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <string>

class Shader {
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);

    void use() const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setInt(const std::string &name, int value) const; // Utile pour les textures
    
    ~Shader();

    GLuint get_id();

private:
    GLuint glid;
    GLuint compile_shader(const std::string& path, GLenum shader_type);
};

#endif // SHADER_H
