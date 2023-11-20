/**
 * @file MainWindow.cpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Implementations
 * @version 0.3
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "MainWindow.hpp"

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "Disklist", wxDefaultPosition, wxSize(1050, 600)) {
    wxMenu* menuFile = new wxMenu;
    this->SetMinSize(wxSize(800, 600));
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("...");

    Bind(wxEVT_MENU, &MainWindow::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    iwindow = new InfoWindow(this, wxID_ANY, wxPoint(800, 0), wxSize(250, 600));
    lwindow = new ListingWindow(this, iwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    sizer->Add(lwindow, wxEXPAND);
    sizer->Add(iwindow);
    
    SetSizer(sizer);
}

void MainWindow::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}