/**
 * @file MainWindow.cpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Implementations
 * @version 0.3
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "MainWindow.hpp"

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "Hello World", wxDefaultPosition, wxSize(800, 600)) {
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
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MainWindow::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);

    lwindow = new ListingWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
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