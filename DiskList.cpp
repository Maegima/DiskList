/**
 * @file DiskList.cpp
 * @author André Lucas Maegima
 * @brief Folder listing program with md5 checksum.
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <list>
#include <iostream>
#include "FileInfo.hpp"

using namespace std;

int main(int argc, char **argv){
    filesystem::path current_path = filesystem::current_path();
    if(argc > 1){
        current_path.append(argv[1]);
    }
    list<FileInfo> fileList;
    for(auto const &entry : filesystem::directory_iterator{current_path}){
        fileList.push_back(FileInfo(entry));
    }
    for(auto file : fileList){
        cout << file << endl;
    } 
    return 0;
}