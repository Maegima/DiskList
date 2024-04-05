/**
 * @file MainWindow.cpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Implementations
 * @version 0.4
 * @date 2024-04-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "MainWindow.hpp"

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "Disklist", wxDefaultPosition, wxSize(1200, 600)) {
    iwindow = new InfoWindow(this, wxID_ANY, wxPoint(800, 0), wxSize(250, 600));
    lwindow = new ListingWindow(this, iwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    wxBoxSizer *windowSizer = new wxBoxSizer(wxHORIZONTAL);
    windowSizer->Add(lwindow, wxEXPAND);
    windowSizer->Add(iwindow);

    wxBoxSizer *toolbarSizer = new wxBoxSizer(wxHORIZONTAL);
    toolbarSizer->AddSpacer(3);
    toolbarSizer->Add(lwindow->backward, 0, wxALL);
    toolbarSizer->AddSpacer(1);
    toolbarSizer->Add(lwindow->forward, 0, wxALL);
    toolbarSizer->AddSpacer(3);
    toolbarSizer->Add(lwindow->breadcrumbs, 1, wxEXPAND);
    toolbarSizer->AddSpacer(3);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(toolbarSizer, 0, wxEXPAND);
    sizer->Add(windowSizer, 1, wxEXPAND);

    SetSizer(sizer);

    CreateStatusBar();
    SetStatusText("...");
}
