/**
 * @file FileSystem.hpp
 * @author your name (you@domain.com)
 * @brief FileSystem utility class
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _FILESYSTEM_HPP_
#define _FILESYSTEM_HPP_

#include <list>
#include <filesystem>
#include "Configuration.hpp"

class FileSystem {
   public:
    class Result {
       private:
       public:
        std::list<std::string> errors;

        inline bool success() const { return this->errors.size() == 0; };
        inline operator bool() const { return success(); };
    };
    static Result OrganizeFolder(const std::filesystem::path &path, const Configuration &config);
    static Result UnwindFolder(const std::filesystem::path &path);

   private:
    static std::list<std::filesystem::path> GetFiles(const std::filesystem::path &path);
    static std::list<std::filesystem::path> GetNames(const std::filesystem::path &path);
    static void MoveEntry(const std::filesystem::path &entry, const Configuration &config);
};

#endif  // _FILESYSTEM_HPP_