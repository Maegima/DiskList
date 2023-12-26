/**
 * @file DiskList.cpp
 * @author André Lucas Maegima
 * @brief Folder listing program with md5 checksum.
 * @version 0.3
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <wx/wx.h>
#include "Application.hpp"
#include "Models/DBTable/FileEntry.hpp"
#include "Controllers/FileInfo.hpp"
#include <list>

void add_file(std::list<FileInfo> &fs, const std::filesystem::path &path) {
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        fs.push_back(FileInfo(entry));
        if (entry.is_directory()) {
            add_file(fs, entry.path());
        }
    }
}

void entry_move(const std::filesystem::directory_entry &entry) {
    std::string extension = entry.path().extension().string();
    std::filesystem::path path = entry.path().parent_path();
    if (extension == ".zip" || extension == ".7z" || extension == ".rar") {
        path /= "zip";
    } else if (extension == ".zipmod") {
        path /= "mod";
    } else if (extension == ".png") {
        path /= "png";
    } else {
        path /= "msc";
    }
    path /= entry.path().filename().c_str();
    std::filesystem::rename(entry.path(), path);
}

void organize_file(const std::filesystem::path &path) {
    std::filesystem::path folder = path;
    std::filesystem::create_directory(folder.string() + "/zip");
    std::filesystem::create_directory(folder.string() + "/png");
    std::filesystem::create_directory(folder.string() + "/mod");
    std::filesystem::create_directory(folder.string() + "/msc");
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        std::filesystem::path filename = entry.path().filename();
        if (entry.is_regular_file()) {
            try {
                entry_move(entry);
            } catch (std::filesystem::filesystem_error &e) {
            }
        } else if (entry.is_directory() && filename != "zip" && filename != "png" && filename != "mod" && filename != "msc") {
            organize_file(entry.path());
        }
    }
}

// wxIMPLEMENT_APP(MyApp);

wxAppInitializer wxTheAppInitializer((wxAppInitializerFunction) new Application);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        wxImage::AddHandler(new wxPNGHandler());
        wxImage::AddHandler(new wxJPEGHandler());
        return wxEntry(argc, argv);
    }
    std::string command = argv[1];
    if (command == "createdb") {
        sqlite3 *db;

        int rc = sqlite3_open(".database.sqlite", &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return 0;
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }

        FileEntry fe = FileEntry(db);
        if (!fe.create()) {
            fprintf(stderr, "SQL error: %s\n", fe.error.c_str());
        }

        sqlite3_close(db);
    } else if (command == "scanfiles") {
        sqlite3 *db;

        int rc = sqlite3_open(".database.sqlite", &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return 0;
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }

        FileEntry fe = FileEntry(db);

        std::filesystem::path current_folder = argc > 2 ? argv[2] : "";
        std::list<FileInfo> fl;
        add_file(fl, current_folder);

        for (auto const &entry : fl) {
            fe.accessed = (unsigned char *)entry.accessed_str().c_str();
            fe.modified = (unsigned char *)entry.modified_str().c_str();
            fe.created = (unsigned char *)entry.accessed_str().c_str();
            fe.size = entry.size;
            fe.hash = (unsigned char *)entry.md5sumString().c_str();
            fe.type = entry.type;
            fe.path = (unsigned char *)entry.path.c_str();
            if (!fe.save()) {
                fprintf(stderr, "SQL error: %s for %s\n", fe.error.c_str(), fe.path.value);
            }
        }
        sqlite3_close(db);
    } else if (command == "organize") {
        std::filesystem::path current_folder = argc > 2 ? argv[2] : "";
        organize_file(current_folder);
    }

    return 0;
}
