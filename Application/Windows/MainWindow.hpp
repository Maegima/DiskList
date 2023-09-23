/**
 * @file MainWindow.hpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Definitions
 * @version 0.2
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_MAINWINDOW_HPP
#define DISKLIST_MAINWINDOW_HPP

#include <SDL.h>
#include <SDL_opengl.h>
#include <filesystem>
#include <list>
#include "Controllers/DirectoryEntry.hpp"

class MainWindow {
   public:
    MainWindow(SDL_Window *window);
    void Loop();

   protected:
    SDL_Window *window;
    bool done;
    std::filesystem::path current_folder;
    std::list<DirectoryEntry*> *directory_entries;
    float *clear_color;

    void ProcessEvent();
    void NewFrame();
    void Render();
    void LoadDirectoryEntries();
    void ChangeDirectoryEntries(std::filesystem::path new_path);
};

#endif /* DISKLIST_MAINWINDOW_HPP_ */
