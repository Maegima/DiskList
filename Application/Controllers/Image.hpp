/**
 * @file Image.hpp
 * @author André Lucas Maegima
 * @brief Image texture loader definition
 * @version 0.2
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_IMAGE_HPP
#define DISKLIST_IMAGE_HPP

#include <SDL.h>
#include <SDL_opengl.h>

class Image {
   public:
    int width;
    int height;

    Image(const char *path);
    ~Image();
    void* GetTexture();

   protected:
    GLuint texture;
};

#endif /* DISKLIST_IMAGE_HPP */