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
    }

    return 0;
}
