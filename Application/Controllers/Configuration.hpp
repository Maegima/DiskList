/**
 * @file Configuration.hpp
 * @author André Lucas Maegima
 * @brief Configuration file class definition
 * @version 0.3
 * @date 2023-11-20
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
    std::map<std::string, std::pair<std::string, int>> folder;
    std::map<std::string, std::vector<std::string>> organize;
    std::map<std::string, std::string> image;

    Configuration(const std::string path);

   private:
    std::fstream file;
    std::map<std::string, std::string> ReadKeysValues();
};

#endif  // _CONFIGURATION_HPP_