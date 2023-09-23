/**
 * @file DirectoryEntry.cpp
 * @author André Lucas Maegima
 * @brief Class with directory entry informations for display.
 * @version 0.2
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "DirectoryEntry.hpp"
#include <iostream>
#include <fstream>

DirectoryEntry::DirectoryEntry(std::filesystem::directory_entry entry) {
    this->path = entry.path();
    this->is_directory = entry.is_directory();
    if (entry.is_directory()) {
        this->text_color = new float[]{0.20f, 1.0f, 1.0f, 1.0f};
        this->img = default_icons["folder"];
    } else {
        this->text_color = new float[]{1.0f, 1.0f, 1.0f, 1.0f};
        auto ext = entry.path().extension().string();
        this->img = default_icons.contains(ext) ? default_icons[ext] : default_icons["default"];
        if (ext == ".png" || ext == ".jpg") {
            this->loaded_img = new Image(entry.path().c_str());
            this->img = this->loaded_img;
        }
    }
}

void DirectoryEntry::InitializeDefaultIcons(const char* path) {
    std::fstream file(path, std::ios::in);
    std::string data;
    DirectoryEntry::default_icons = std::map<std::string, Image*>();
    while (getline(file, data)) {
        size_t pos = data.find('=');
        if (pos != std::string::npos) {
            Image* img = new Image(data.substr(pos + 1).c_str());
            std::string ext = data.substr(0, pos);
            default_icons.insert({ext, img});
            std::cout << data << " " << img->GetTexture() << " " << img->height << " " << img->width << std::endl;
        }
    }
    
}

DirectoryEntry::~DirectoryEntry() {
    if(this->loaded_img) 
        this->loaded_img->~Image();
}
