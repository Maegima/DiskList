/**
 * @file FileInfo.cpp
 * @author André Lucas Maegima
 * @brief Class to extract file infomations.
 * @version 0.3
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <string>
#include <filesystem>
#include <sys/stat.h>
#include "md5/md5.hpp"
#include "FileInfo.hpp"

FileInfo::FileInfo(filesystem::directory_entry entry, bool with_hash){
    path = entry.path();
    struct stat f_stat;
    stat(path.c_str(), &f_stat);
    type = S_ISDIR(f_stat.st_mode) ? FileType::Directory : FileType::File;
    size = f_stat.st_size;
    created = f_stat.st_ctim.tv_sec;
    modified = f_stat.st_mtim.tv_sec;
    accessed = f_stat.st_atim.tv_sec;
    Hash *hash = new Hash();
    if(type == FileType::File && with_hash){
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

std::string FileInfo::size_str() const {
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

std::string FileInfo::created_str() const {
    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&created));
    return std::string(buff);
}

std::string FileInfo::modified_str() const {
    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&modified));
    return std::string(buff);
}

std::string FileInfo::accessed_str() const {
    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&accessed));
    return std::string(buff);
}

std::string FileInfo::type_str() const {
    return type == FileType::Directory ? "Directory" : "File";
}

string FileInfo::md5sumString() const {
    std::stringstream ss;
    for(int i=0; i<16; ++i)
        ss << setfill('0') << setw(2) << std::hex << (int)md5sum[i];
    return ss.str();
}

string FileInfo::to_string() const {
    return created_str() + " " + modified_str() + " " + accessed_str() +
    " " + md5sumString() + " " + size_str() + " " + type_str() + " " + path.string();
}

ostream& operator<<(ostream& os, const FileInfo& file){
    return os << file.to_string();
}