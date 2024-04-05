/**
 * @file Application.cpp
 * @author André Lucas Maegima
 * @brief Disklist Application override implementations
 * @version 0.4
 * @date 2024-04-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Application.hpp"
#include "Windows/ListingWindow.hpp"

bool Application::OnInit() {
    wxLog::SetActiveTarget(new wxLogStderr());
    ListingWindow* window = new ListingWindow(nullptr, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    window->Show(true);
    return true;
}