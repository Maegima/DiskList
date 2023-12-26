/**
 * @file FileSystem.hpp
 * @author André Lucas Maegima
 * @brief FileSystem utility class
 * @version 0.3
 * @date 2023-12-26
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
    static Result Move(const std::filesystem::path &path, const std::filesystem::path &folder);
    static Result OrganizeFolder(const std::filesystem::path &path, const Configuration &config);
    static Result OrganizeFolder(const std::filesystem::path &root, const std::filesystem::path &path, const Configuration &config);
    static Result UnwindFolder(const std::filesystem::path &path);
    static Result DeleteEmptyFolders(const std::filesystem::path &path);

   private:
    static std::list<std::filesystem::path> GetFiles(const std::filesystem::path &path);
    static std::list<std::filesystem::path> GetNames(const std::filesystem::path &path);
    static std::string GetUniqueName(const std::list<std::filesystem::path> &names, const std::filesystem::path &file);
    static std::string GetOrganizerFolder(const std::filesystem::path &entry, const Configuration &config);
};

#endif  // _FILESYSTEM_HPP_