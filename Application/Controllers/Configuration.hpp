/**
 * @file Configuration.hpp
 * @author André Lucas Maegima
 * @brief Configuration file class definition
 * @version 0.4
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _CONFIGURATION_HPP_
#define _CONFIGURATION_HPP_

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <wx/image.h>

class Configuration {
   public:
    std::map<std::string, std::string> config;
    std::map<int, std::pair<std::string, std::string>> folder;
    std::map<std::string, std::vector<std::string>> organize;
    std::map<std::string, wxImage*> image;
    std::vector<std::string> image_extension;
    std::vector<std::pair<std::string, std::string>> file_info;

    Configuration(const std::string path);

   private:
    std::fstream file;
    std::vector<std::pair<std::string, std::string>> ReadKeysValues();
};

#endif  // _CONFIGURATION_HPP_