/**
 * @file InfoWindow.cpp
 * @author André Lucas Maegima
 * @brief Information Window class implementation
 * @version 0.4
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "InfoWindow.hpp"

InfoWindow::InfoWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, wxID_ANY, pos, size) {
    SetBackgroundColour(*wxWHITE);
    SetSizer(new wxBoxSizer(wxVERTICAL));
}

wxPanel* InfoWindow::CreateCenteredText(wxString label, wxSize size) {
    wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, size);
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    auto text = new wxStaticText(panel, wxID_ANY, label);
    text->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    int max_text_size = 240;
    while (text->m_width > 240) {
        int parts = text->m_width / max_text_size;
        int size = label.length() / (parts + 1);
        wxString newstr = "";
        for (int i = 0; i < parts; i++) {
            newstr += label.substr(i * size, size) + "\n";
        }
        newstr += label.substr(parts * size);
        text->SetLabel(newstr);
        max_text_size -= 10;
    }
    panel->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
    sizer->Add(text, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
    panel->SetSizer(sizer);
    return panel;
}

void InfoWindow::FillGrid(std::list<std::pair<wxString, wxString>> lines) {
    auto sizer = this->GetSizer();
    sizer->Clear(true);
    for (auto const& pair : lines) {
        wxBoxSizer* line = new wxBoxSizer(wxHORIZONTAL);
        line->Add(CreateCenteredText(pair.first, wxSize(60, 24)), 0, wxEXPAND | wxTOP | wxRIGHT, 4);
        line->Add(CreateCenteredText(pair.second), 1, wxEXPAND | wxTOP | wxRIGHT, 4);
        sizer->Add(line, 0, wxEXPAND | wxLEFT, 4);
    }
    this->SendSizeEvent();
    this->Refresh();
}