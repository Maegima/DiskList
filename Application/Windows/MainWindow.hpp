/**
 * @file MainWindow.hpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Definitions
 * @version 0.2
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_MAINWINDOW_HPP
#define DISKLIST_MAINWINDOW_HPP

#include <SDL.h>
#include <SDL_opengl.h>

class MainWindow {
   public:
    MainWindow(SDL_Window *window);

   protected:
    SDL_Window *window;

};

#endif /* DISKLIST_MAINWINDOW_HPP_ */
