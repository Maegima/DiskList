/**
 * @file MainWindow.cpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Implementations
 * @version 0.4
 * @date 2024-04-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "MainWindow.hpp"

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Disklist", wxDefaultPosition, wxSize(1200, 600)) {
    lwindow = new ListingWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(lwindow->CreateToolbarSizer(), 0, wxEXPAND);
    sizer->Add(lwindow->CreateWindowSizer(), 1, wxEXPAND);

    SetSizer(sizer);

    CreateStatusBar();
}
