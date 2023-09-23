/**
 * @file DirectoryEntry.hpp
 * @author André Lucas Maegima
 * @brief Class with directory entry informations for display.
 * @version 0.2
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _DIRECTORYENTRY_HPP_
#define _DIRECTORYENTRY_HPP_

#include "Image.hpp"
#include <map>
#include <filesystem>

class DirectoryEntry {
   public:
    Image *img;
    float *text_color;
    std::filesystem::path path;
    bool is_directory;

    DirectoryEntry(std::filesystem::directory_entry entry);
    ~DirectoryEntry();
    static void InitializeDefaultIcons(const char *path);

   private:
    inline static std::map<std::string, Image *> default_icons;
    Image *loaded_img = nullptr;
};

#endif  // _DIRECTORYENTRY_HPP_