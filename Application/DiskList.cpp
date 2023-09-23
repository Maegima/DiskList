/**
 * @file DiskList.cpp
 * @author André Lucas Maegima
 * @brief Folder listing program with md5 checksum.
 * @version 0.2
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Application.hpp"
#include "Windows/MainWindow.hpp"

int main(int argc, char* argv[]) {
    auto app = Application();
    auto mainWindow = MainWindow(app.window);
    mainWindow.Loop();
    return 0;
}
