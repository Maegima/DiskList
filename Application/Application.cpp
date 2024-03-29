/**
 * @file Application.cpp
 * @author André Lucas Maegima
 * @brief Disklist Application override implementations
 * @version 0.3
 * @date 2024-03-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Application.hpp"
#include "Windows/MainWindow.hpp"

bool Application::OnInit() {
    wxLog::SetActiveTarget(new wxLogStderr());
    MainWindow* window = new MainWindow();
    window->Show(true);
    return true;
}