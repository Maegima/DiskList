/**
 * @file FileInfo.hpp
 * @author André Lucas Maegima
 * @brief Class to extract file infomations.
 * @version 0.1
 * @date 2023-07-30
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
    std::string path;
    off_t size;
    long created;
    long modified;
    long accessed;
    uint8_t* md5sum;
public:
    FileType type;
    FileInfo(std::filesystem::directory_entry entry);
    std::string size_to_string(size_t size) const;
    std::string to_string() const; 
};

std::ostream& operator<<(std::ostream& os, const FileInfo& file);

#endif
