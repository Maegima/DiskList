/**
 * @file FileInfo.hpp
 * @author André Lucas Maegima
 * @brief Class to extract file infomations.
 * @version 0.2
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _FILEINFO_HPP_
#define _FILEINFO_HPP_

#include <string>
#include <filesystem>

enum FileType {Directory, File};

class FileInfo {
private:
    off_t size;
    long created;
    long modified;
    long accessed;
    uint8_t* md5sum;
public:
    std::filesystem::path path;
    FileType type;
    FileInfo(std::filesystem::directory_entry entry);
    std::string size_to_string(size_t size) const;
    std::string to_string() const; 
    std::string md5sumString() const;
};

std::ostream& operator<<(std::ostream& os, const FileInfo& file);

#endif
