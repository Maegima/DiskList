/**
 * @file Image.cpp
 * @author André Lucas Maegima
 * @brief Image texture loader implementation
 * @version 0.2
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Image.hpp"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(const char* path) {
    unsigned char* image_data = stbi_load(path, &this->width, &this->height, NULL, 4);
    if (image_data == NULL)
        throw std::runtime_error(stbi_failure_reason());

    // Create a OpenGL texture identifier
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  // Same

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    stbi_image_free(image_data);
}

Image::~Image() {
    // Delete a OpenGL texture identifier
    glDeleteTextures(1, &this->texture);
}

void* Image::GetTexture() {
    return (void*)(intptr_t)texture;
}
