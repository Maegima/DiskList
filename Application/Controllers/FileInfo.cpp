/**
 * @file FileInfo.cpp
 * @author André Lucas Maegima
 * @brief Class to extract file infomations.
 * @version 0.2
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <string>
#include <filesystem>
#include <sys/stat.h>
#include "md5/md5.hpp"
#include "FileInfo.hpp"

FileInfo::FileInfo(filesystem::directory_entry entry){
    path = entry.path();
    struct stat f_stat;
    stat(path.c_str(), &f_stat);
    type = S_ISDIR(f_stat.st_mode) ? FileType::Directory : FileType::File;
    size = f_stat.st_size;
    created = f_stat.st_ctim.tv_sec;
    modified = f_stat.st_mtim.tv_sec;
    accessed = f_stat.st_atim.tv_sec;
    Hash *hash = new Hash();
    if(type == FileType::File){
        fstream file(path, std::ios::binary | std::ios::in);
        if(file){
            md5sum = (uint8_t*) hash->md5(&file, size);
        } else {
            md5sum = (uint8_t*) hash->md5("");
        }
    }
    else{
        md5sum = (uint8_t*) hash->md5("");
    }
}
string FileInfo::size_to_string(size_t size) const{
    std::stringstream ss;
    ss << setfill(' ');
    if(size >= 1000000000){
        ss << setw(4) << setprecision(3) << (float)size/1000000000 << "GB";
    }
    else if(size >= 1000000){
        ss << setw(4) << setprecision(3) << (float)size/1000000 << "MB";
    }
    else if(size >= 5000){
        ss << setw(4) << setprecision(3) << (float)size/1000 << "KB";
    }
    else{
        ss << setw(5) << size << "B";
    }
    return ss.str();
}

string FileInfo::md5sumString() const {
    std::stringstream ss;
    for(int i=0; i<16; ++i)
        ss << setfill('0') << setw(2) << std::hex << (int)md5sum[i];
    return ss.str();
}

string FileInfo::to_string() const {
    return std::to_string(type) + " " + std::to_string(created) + " " + std::to_string(modified) + " " + std::to_string(accessed) +
    " " + md5sumString() + " " + size_to_string(size) + " " + path.string();
}

ostream& operator<<(ostream& os, const FileInfo& file){
    return os << file.to_string();
}