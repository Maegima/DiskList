/**
 * @file FileSystem.cpp
 * @author André Lucas Maegima
 * @brief FileSystem utility class
 * @version 0.3
 * @date 2023-12-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "FileSystem.hpp"
#include <algorithm>

FileSystem::Result FileSystem::Move(const std::filesystem::path &path, const std::filesystem::path &folder) {
    FileSystem::Result result;
    std::filesystem::path new_path = folder / path.filename();
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

FileSystem::Result FileSystem::OrganizeFolder(const std::filesystem::path &path, const Configuration &config) {
    FileSystem::Result result;
    std::filesystem::path folder = path;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        std::filesystem::path filename = entry.path().filename();
        if (entry.is_regular_file()) {
            try {
                MoveEntry(entry, config);
            } catch (std::filesystem::filesystem_error &e) {
                result.errors.push_back(entry.path().filename().string() + ": " + e.what());
            }
        } else if (entry.is_directory() && !config.organize.contains(filename)) {
            result.errors.splice(result.errors.end(), OrganizeFolder(entry, config).errors);
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
        int count = 1;
        std::string stem = file.filename().stem();
        std::string extension = file.filename().extension();
        std::string filename = file.filename();
        while (std::find(current_names.begin(), current_names.end(), filename) != current_names.end()) {
            filename = stem + "(" + std::to_string(count++) + ")" + extension;
        }
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

std::list<std::filesystem::path> FileSystem::GetFiles(const std::filesystem::path &path) {
    std::list<std::filesystem::path> files;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        if (entry.is_regular_file()) {
            files.push_back(entry);
        } else if (entry.is_directory()) {
            files.splice(files.end(), GetFiles(entry));
        }
    }
    return files;
}

std::list<std::filesystem::path> FileSystem::GetNames(const std::filesystem::path &path) {
    std::list<std::filesystem::path> names;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        std::filesystem::path filename = entry.path().filename();
        names.push_back(filename);
    }
    return names;
}

void FileSystem::MoveEntry(const std::filesystem::path &entry, const Configuration &config) {
    std::string extension = entry.extension().string();
    std::filesystem::path path = entry.parent_path();
    for (const auto &[key, list] : config.organize) {
        if (std::find(list.begin(), list.end(), extension) != list.end()) {
            path /= key;
            break;
        }
    }
    path /= entry.filename().c_str();
    if (path != entry) {
        if (!std::filesystem::exists(path.parent_path())) {
            std::filesystem::create_directory(path.parent_path());
        }
        std::filesystem::rename(entry, path);
    }
}
