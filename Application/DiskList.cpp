/**
 * @file DiskList.cpp
 * @author André Lucas Maegima
 * @brief Folder listing program with md5 checksum.
 * @version 0.2
 * @date 2023-09-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Application.hpp"
#include "Windows/MainWindow.hpp"

int main(int argc, char* argv[]) {
    Application app = Application();
    MainWindow(app.window);
    return 0;
}
