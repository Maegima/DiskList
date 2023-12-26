/**
 * @file Configuration.hpp
 * @author André Lucas Maegima
 * @brief Configuration file class definition
 * @version 0.3
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _CONFIGURATION_HPP_
#define _CONFIGURATION_HPP_

#include <string>
#include <map>
#include <vector>
#include <fstream>

class Configuration {
   public:
    std::map<std::string, std::string> config;
    std::map<int, std::pair<std::string, std::string>> folder;
    std::map<std::string, std::vector<std::string>> organize;
    std::map<std::string, std::string> image;
    std::vector<std::string> image_extension;

    Configuration(const std::string path);

   private:
    std::fstream file;
    std::map<std::string, std::string> ReadKeysValues();
};

#endif  // _CONFIGURATION_HPP_