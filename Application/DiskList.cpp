/**
 * @file DiskList.cpp
 * @author André Lucas Maegima
 * @brief Folder listing program with md5 checksum.
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Application.hpp"
#include "Windows/MainWindow.hpp"
#include "Models/DBTable/FileEntry.hpp"
#include "Controllers/FileInfo.hpp"
#include <list>

int main(int argc, char* argv[]) {
    if(argc == 1){
        auto app = Application();
        auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        app.Run(new MainWindow("Disklist", 1280, 720, window_flags));
    }
    std::string command = argv[1];
    if(command == "createdb"){
        sqlite3 *db;

        int rc = sqlite3_open(".database.sqlite", &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return 0;
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }

        FileEntry fe = FileEntry(db);
        if(!fe.create()){
            fprintf(stderr, "SQL error: %s\n", fe.error.c_str());
        }

        sqlite3_close(db);
    } else if(command == "scanfiles") {
        sqlite3 *db;

        int rc = sqlite3_open(".database.sqlite", &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return 0;
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }

        FileEntry fe = FileEntry(db);

        std::filesystem::path current_folder = std::filesystem::current_path();
        std::list<FileInfo> fl;
        for (auto const &entry : std::filesystem::directory_iterator{current_folder}) {
            fl.push_back(FileInfo(entry));
        }

        char buff[20];
        for( auto const &entry : fl){
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&entry.accessed));
            fe.accessed = (unsigned char *) buff;
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&entry.modified));
            fe.modified = (unsigned char *) buff;
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&entry.created));
            fe.created = (unsigned char *) buff;
            fe.size = entry.size;
            fe.hash = (unsigned char *) entry.md5sumString().c_str();
            fe.type = entry.type;
            fe.path = (unsigned char *) entry.path.c_str();
            if(!fe.save()){
                fprintf(stderr, "SQL error: %s for %s\n", fe.error.c_str(), fe.path.value);
            }
        }
        sqlite3_close(db);
    }

    return 0;
}
