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
    wxImage rightImage(wxT("/home/lucas/Git/myself/DiskList/.images/chevrons-right.png"), wxBITMAP_TYPE_PNG);
    wxImage leftImage(wxT("/home/lucas/Git/myself/DiskList/.images/chevrons-left.png"), wxBITMAP_TYPE_PNG);

    forward = new wxBitmapButton(this, wxID_FORWARD, rightImage, wxDefaultPosition, wxSize(32, 32), wxBORDER_NONE);
    backward = new wxBitmapButton(this, wxID_BACKWARD, leftImage, wxDefaultPosition, wxSize(32, 32), wxBORDER_NONE);
    forward->SetBitmapHover(rightImage.ChangeLightness(170));
    backward->SetBitmapHover(leftImage.ChangeLightness(170));

    iwindow = new InfoWindow(this, wxID_ANY, wxPoint(800, 0), wxSize(250, 600));
    lwindow = new ListingWindow(this, iwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    wxBoxSizer *windowSizer = new wxBoxSizer(wxHORIZONTAL);
    windowSizer->Add(lwindow, wxEXPAND);
    windowSizer->Add(iwindow);

    wxBoxSizer *toolbarSizer = new wxBoxSizer(wxHORIZONTAL);
    toolbarSizer->AddSpacer(3);
    toolbarSizer->Add(backward, 0, wxALL);
    toolbarSizer->AddSpacer(1);
    toolbarSizer->Add(forward, 0, wxALL);
    toolbarSizer->AddSpacer(3);
    toolbarSizer->Add(new wxTextCtrl( this, -1, "My text.", wxDefaultPosition, wxSize(100,32)), 1, wxEXPAND);
    toolbarSizer->AddSpacer(3);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(toolbarSizer, 0, wxEXPAND);
    sizer->Add(windowSizer, 1, wxEXPAND);
    
    SetSizer(sizer);

    CreateStatusBar();
    SetStatusText("...");
    
    Bind(wxEVT_BUTTON, &MainWindow::OnBackward, this, wxID_BACKWARD);
    Bind(wxEVT_BUTTON, &MainWindow::OnForward, this, wxID_FORWARD);
    Bind(wxEVT_AUX1_DOWN, &MainWindow::OnBackward, this);
    Bind(wxEVT_AUX2_DOWN, &MainWindow::OnForward, this);
    Bind(wxEVT_CHAR_HOOK, &MainWindow::OnKeyPress, this);
}

void MainWindow::OnBackward(wxEvent &event) {
    lwindow->GoBackward();
    
}

void MainWindow::OnForward(wxEvent &event) {
    lwindow->GoForward();
}

void MainWindow::OnKeyPress(wxKeyEvent &event) {
    if (event.AltDown()) {
        switch (event.GetKeyCode()) {
            case WXK_LEFT:
                OnBackward(event);
                break;
            case WXK_RIGHT:
                OnForward(event);
                break;
        }
    }
}
