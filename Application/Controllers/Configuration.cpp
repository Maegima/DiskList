/**
 * @file Configuration.cpp
 * @author André Lucas Maegima
 * @brief Configuration file class implementation
 * @version 0.3
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include "Configuration.hpp"
#include "Algorithm.hpp"

Configuration::Configuration(const std::string path) : file(std::fstream(path, std::ios::in)) {
    std::string data;
    while (getline(file, data)) {
        size_t space_ini = data.find('[');
        size_t space_end = data.find(']');
        if (space_ini != std::string::npos && space_end != std::string::npos) {
            std::string space = data.substr(space_ini + 1, space_end - space_ini - 1);
            auto items = ReadKeysValues();
            std::cout << "[" << space << "]" << std::endl;
            for (auto &[key, value] : items) {
                std::cout << key << "=" << value << std::endl;
            }
            if (space == "config") {
                for (auto &[key, value] : items) {
                    config.insert({key, value});
                }
            } else if (space == "folder") {
                for (auto &[key, value] : items) {
                    size_t pos = value.find("_");
                    std::string name = value.substr(0, pos);
                    int id = stoi(value.substr(pos + 1));
                    folder.insert({id, {key, name}});
                }
            } else if (space == "organize") {
                for (auto &[key, value] : items) {
                    organize.insert({key, Algorithm::split<std::vector>(value, ',')});
                }
            } else if (space == "image") {
                for (auto &[key, value] : items) {
                    if(key == "dynamic") {
                        image_extension = Algorithm::split<std::vector>(value, ',');
                    } else {
                        image.insert({key, new wxImage(value, wxBITMAP_TYPE_PNG)});
                    }
                }
            } else if (space == "fileinfo") {
                file_info = items;
            }
        }
    }
}

std::vector<std::pair<std::string, std::string>> Configuration::ReadKeysValues() {
    bool end = false;
    std::string data;
    std::vector<std::pair<std::string, std::string>> items;
    while (!end && getline(file, data)) {
        size_t pos = data.find('=');
        if (pos != std::string::npos) {
            std::string key = data.substr(0, pos);
            std::string value = data.substr(pos + 1);
            items.push_back({key, value});
        }
        end = data.empty();
    }
    return items;
}