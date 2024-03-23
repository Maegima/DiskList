/**
 * @file FileSystem.cpp
 * @author André Lucas Maegima
 * @brief FileSystem utility class
 * @version 0.3
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "FileSystem.hpp"
#include "Algorithm.hpp"

FileSystem::Result FileSystem::Move(const std::filesystem::path &path, const std::filesystem::path &folder) {
    FileSystem::Result result;
    auto filename = GetUniqueName(GetNames(folder), path);
    std::filesystem::path new_path = folder / filename;
    try {
        if (!std::filesystem::exists(folder)) {
            std::filesystem::create_directory(folder);
        }
        if (!std::filesystem::exists(new_path)) {
            std::filesystem::rename(path, new_path);
        } else {
            result.errors.push_back(path.filename().string() + ": failed to move!");
        }
    } catch (const std::filesystem::filesystem_error &e) {
        result.errors.push_back(path.filename().string() + ": " + e.what());
    }
    return result;
}

FileSystem::Result FileSystem::OrganizeCurrentFolder(const std::filesystem::path &path, const Configuration &config) {
    return OrganizeFolder(path, path, config, false);
}

FileSystem::Result FileSystem::OrganizeFolder(const std::filesystem::path &path, const Configuration &config) {
    return OrganizeFolder(path, path, config);
}

FileSystem::Result FileSystem::OrganizeFolder(const std::filesystem::path &root, const std::filesystem::path &path, const Configuration &config, bool recursive) {
    FileSystem::Result result;
    std::map<std::string, std::list<std::filesystem::path> *> folder_names;
    auto files_to_organize = GetFiles(path, recursive);
    for (auto const &file : files_to_organize) {
        try {
            auto move_folder = GetOrganizerFolder(file, config);
            auto move_path = move_folder != "" ? root / move_folder : root;
            if (!folder_names.contains(move_folder)) {
                if (!std::filesystem::exists(move_path)) {
                    std::filesystem::create_directory(move_path);
                }
                auto listname = GetNames(move_path);
                folder_names[move_folder] = new std::list<std::filesystem::path>(listname);
            }
            auto current_names = folder_names[move_folder];
            if (move_path != file.parent_path()) {
                auto filename = GetUniqueName(*current_names, file);
                move_path /= filename;
                std::filesystem::rename(file, move_path);
                current_names->push_back(filename);
            }
        } catch (std::filesystem::filesystem_error &e) {
            result.errors.push_back(file.filename().string() + ": " + e.what());
        }
    }
    return result;
}

FileSystem::Result FileSystem::UnwindFolder(const std::filesystem::path &path) {
    FileSystem::Result result;
    auto parent = path.parent_path();
    auto files_to_unwind = GetFiles(path);
    auto current_names = GetNames(parent);
    for (auto const &file : files_to_unwind) {
        auto filename = GetUniqueName(current_names, file);
        current_names.push_back(filename);
        std::filesystem::path new_path = parent / filename;
        try {
            if (!std::filesystem::exists(new_path)) {
                std::filesystem::rename(file, new_path);
            } else {
                result.errors.push_back(new_path.string() + " exists!");
            }
        } catch (std::filesystem::filesystem_error &e) {
            result.errors.push_back(filename + ": " + e.what());
        }
    }
    return result;
}

FileSystem::Result FileSystem::DeleteEmptyFolders(const std::filesystem::path &path) {
    FileSystem::Result result;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        if (entry.is_directory()) {
            result.errors.splice(result.errors.end(), DeleteEmptyFolders(entry).errors);
            try {
                if (std::filesystem::is_empty(entry)) {
                    std::filesystem::remove(entry);
                }
            } catch (const std::filesystem::filesystem_error &e) {
                result.errors.push_back(entry.path().filename().string() + ": " + e.what());
            }
        }
    }
    return result;
}

std::list<std::filesystem::path> FileSystem::GetFiles(const std::filesystem::path &path, bool recursive) {
    std::list<std::filesystem::path> files;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        if (entry.is_regular_file()) {
            files.push_back(entry);
        } else if (entry.is_directory() && recursive) {
            files.splice(files.end(), GetFiles(entry));
        }
    }
    return files;
}

std::list<std::filesystem::path> FileSystem::GetNames(const std::filesystem::path &path) {
    std::list<std::filesystem::path> names;
    if(!std::filesystem::exists(path)){
        return names;
    }
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        std::filesystem::path filename = entry.path().filename();
        names.push_back(filename);
    }
    return names;
}

std::string FileSystem::GetUniqueName(const std::list<std::filesystem::path> &names, const std::filesystem::path &file) {
    std::string filename = file.filename();
    std::string stem = file.filename().stem();
    std::string extension = file.filename().extension();
    int count = 1;
    while (Algorithm::contains(names, filename)) {
        filename = stem + "(" + std::to_string(count++) + ")" + extension;
    }
    return filename;
}

std::string FileSystem::GetOrganizerFolder(const std::filesystem::path &entry, const Configuration &config) {
    std::string extension = entry.extension().string();
    std::filesystem::path path = entry.parent_path();
    for (const auto &[key, list] : config.organize) {
        if (Algorithm::contains(list, extension)) {
            return key;
        }
    }
    return "";
}
