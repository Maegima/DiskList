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
    : wxPanel(parent, wxID_ANY, pos, size),
      grid(CreateGrid()) {
    Bind(wxEVT_SIZE, &InfoWindow::OnSize, this, wxID_ANY);
}

wxGrid* InfoWindow::CreateGrid() {
    wxGrid* newGrid = new wxGrid(this, wxID_ANY);
    newGrid->CreateGrid(0, 1);
    newGrid->SetRowLabelSize(wxGRID_AUTOSIZE);
    newGrid->EnableDragColSize(false);
    newGrid->EnableDragRowSize(false);
    newGrid->EnableEditing(false);
    newGrid->SetColSize(0, this->m_width - newGrid->GetRowLabelSize());
    newGrid->HideColLabels();
    newGrid->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    newGrid->SetLabelFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    return newGrid;
}

void InfoWindow::FillGrid(std::list<std::pair<wxString, wxString>> lines) {
    if (grid->GetNumberRows() > 0) {
        grid->DeleteRows(0, grid->GetNumberRows());
    }
    grid->InsertRows(0, lines.size());
    int row = 0;
    for (auto const& line : lines) {
        grid->SetRowLabelValue(row, line.first);
        grid->SetCellValue(row, 0, line.second);
        row++;
    }
    grid->SetRowLabelSize(wxGRID_AUTOSIZE);
    grid->SetColSize(0, this->m_width - grid->GetRowLabelSize());
}

void InfoWindow::OnSize(wxSizeEvent& event) {
    auto newSize = this->m_parent->GetClientSize();
    this->SetSize(250, newSize.y);
    this->grid->SetSize(this->GetSize());
    Refresh();
    event.Skip();
}