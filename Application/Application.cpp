/**
 * @file Application.cpp
 * @author André Lucas Maegima
 * @brief Disklist Application override implementations
 * @version 0.3
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Application.hpp"
#include "Windows/MainWindow.hpp"

bool Application::OnInit() {
    MainWindow* window = new MainWindow();
    window->Show(true);
    return true;
}