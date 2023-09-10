/**
 * @file MainWindow.cpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Implementations
 * @version 0.2
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <stdexcept>
#include "MainWindow.h"
#include "Controllers/FileInfo.hpp"

std::list<FileInfo> getFileInfoList(std::filesystem::path path) {
    std::list<FileInfo> fileList;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        FileInfo fileInfo = FileInfo(entry);
        fileList.push_back(fileInfo);
        if (fileInfo.type == FileType::Directory) {
            std::list<FileInfo> dirList = getFileInfoList(entry.path());
            fileList.splice(fileList.end(), dirList);
        }
    }
    return fileList;
}

MainWindow::MainWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow(cobject), m_refBuilder(refBuilder) {
    grid_view = Gtk::Builder::get_widget_derived<GridViewImage>(refBuilder, "grid");
    if (!grid_view) 
        throw std::runtime_error("No \"grid\" object in window.ui");
    
    auto entries = getFileInfoList(std::filesystem::current_path());
    for (const auto &entry : entries) 
        grid_view->add_entry(entry.path, entry.md5sumString());
}

void MainWindow::open_file_view(const Glib::RefPtr<Gio::File> & /* file */) {}

MainWindow* MainWindow::create() {
    auto refBuilder = Gtk::Builder::create_from_resource("/resources/disklist/Application/Windows/window.ui");

    auto window = Gtk::Builder::get_widget_derived<MainWindow>(refBuilder, "app_window");
    if (!window) 
        throw std::runtime_error("No \"app_window\" object in window.ui");
    
    return window;
}