/**
 * @file DiskList.cpp
 * @author André Lucas Maegima
 * @brief Folder listing program with md5 checksum.
 * @version 0.1
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <list>
#include <iostream>
#include "FileInfo.hpp"

using namespace std;
 
list<FileInfo> getFileInfoList(filesystem::path path){
    list<FileInfo> fileList;
    for(auto const &entry : filesystem::directory_iterator{path}){
        FileInfo fileInfo = FileInfo(entry);
        fileList.push_back(fileInfo);
        if(fileInfo.type == FileType::Directory){
            list<FileInfo> dirList = getFileInfoList(entry.path());
            fileList.insert(fileList.end(), dirList.begin(), dirList.end());
        }
    }
    return fileList;
}

int main(int argc, char **argv){
    filesystem::path current_path = filesystem::current_path();
    if(argc > 1){
        current_path.append(argv[1]);
    }
    list<FileInfo> fileList = getFileInfoList(current_path);
    for(auto file : fileList){
        cout << file << endl;
    } 
    return 0; 
}