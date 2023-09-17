/**
 * @file Application.hpp
 * @author André Lucas Maegima
 * @brief Disklist Application definitions
 * @version 0.2
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_APPLICATION_HPP
#define DISKLIST_APPLICATION_HPP

#include <SDL.h>
#include <SDL_opengl.h>

class Application {
   protected:
    const char* glsl_version = "#version 130";
    SDL_GLContext gl_context;
    void SetupSDL();
    void CreateWindow();
    void CreateContext();
    void SetupImGui();
    void DestroySDL();
    void DestroyImGui();

   public:
    SDL_Window *window;
    Application();
    ~Application();
};

#endif /* DISKLIST_APPLICATION_HPP */
