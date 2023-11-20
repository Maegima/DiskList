/**
 * @file Configuration.cpp
 * @author André Lucas Maegima
 * @brief Configuration file class implementation
 * @version 0.3
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include "Configuration.hpp"

std::vector<std::string> split(const std::string str, char delimiter) {
    size_t pos = 0;
    std::string token;
    std::string s = str;
    std::vector<std::string> list;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + 1);
        list.push_back(token);
    }
    list.push_back(s);
    return list;
}

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
                config = items;
            } else if (space == "folder") {
                for (auto &[key, value] : items) {
                    size_t pos = value.find("_");
                    std::string name = value.substr(0, pos);
                    int id = stoi(value.substr(pos + 1));
                    folder.insert({key, {value, id}});
                }
            } else if (space == "organize") {
                for (auto &[key, value] : items) {
                    organize.insert({key, split(value, ',')});
                }
            } else if (space == "image") {
                for (auto &[key, value] : items) {
                    image.insert({key, value});
                }
            }
        }
    }
}

std::map<std::string, std::string> Configuration::ReadKeysValues() {
    bool end = false;
    std::string data;
    std::map<std::string, std::string> items;
    while (!end && getline(file, data)) {
        size_t pos = data.find('=');
        if (pos != std::string::npos) {
            std::string key = data.substr(0, pos);
            std::string value = data.substr(pos + 1);
            items.insert({key, value});
        }
        end = data.empty();
    }
    return items;
}