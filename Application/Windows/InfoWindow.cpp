/**
 * @file InfoWindow.cpp
 * @author André Lucas Maegima
 * @brief Information Window class implementation
 * @version 0.3
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "InfoWindow.hpp"

InfoWindow::InfoWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, wxID_ANY, pos, size) {
    SetBackgroundColour(*wxRED);

    Bind(wxEVT_SIZE, &InfoWindow::OnSize, this, wxID_ANY);

    grid = new Grid(this);
}

void InfoWindow::OnSize(wxSizeEvent& event) {
    auto newSize = this->m_parent->GetClientSize();
    this->SetSize(250, newSize.y);
    this->grid->SetSize(this->GetSize());
    Refresh();
    event.Skip();
}
