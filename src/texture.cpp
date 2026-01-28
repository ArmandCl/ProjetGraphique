#include "texture.h"
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& tex_file, GLenum wrap_mode, GLenum min_filter, GLenum mag_filter)
    : glid_(0)
{
    stbi_set_flip_vertically_on_load(true); 

    glGenTextures(1, &glid_);
    glBindTexture(GL_TEXTURE_2D, glid_);

    int width, height, num_channels;
    unsigned char* data = stbi_load(tex_file.c_str(), &width, &height, &num_channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture from file: " << tex_file << std::endl;
        throw std::runtime_error("Failed to load texture from file");
    }

    GLenum format;
    if (num_channels == 1) {
        format = GL_RED;
    } else if (num_channels == 3) {
        format = GL_RGB;
    } else if (num_channels == 4) {
        format = GL_RGBA;
    } else {
        stbi_image_free(data);
        std::cerr << "Unknown texture format: " << num_channels << " channels" << std::endl;
        throw std::runtime_error("Unknown texture format");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    std::cout << "Loaded texture " << tex_file << " (" << width << "x" << height << ")" << std::endl;
}

Texture::~Texture() {
    glDeleteTextures(1, &glid_);
}