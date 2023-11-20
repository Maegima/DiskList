
/*
 * @file InfoWindow.hpp
 * @author André Lucas Maegima
 * @brief Information Window class definition
 * @version 0.3
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _INFOWINDOW_HPP_
#define _INFOWINDOW_HPP_

#include <wx/wx.h>
#include <wx/grid.h>
#include <filesystem>

class Grid : public wxGrid {
   public:
    Grid(wxWindow* parent) : wxGrid(parent, wxID_ANY, wxDefaultPosition) {
        CreateGrid(10, 1);
        SetRowLabelSize(wxGRID_AUTOSIZE);
        EnableDragColSize(false);
        EnableDragRowSize(false);
        EnableEditing(false);
        SetColSize(0, 300 - GetColSize(0));
        HideColLabels();
        SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
        SetLabelFont(wxFont(9, wxFONTFAMILY_DEFAULT,
                                  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        
    }
    void FillGrid(std::filesystem::directory_entry entry) {
        SetCellValue(0, 0, entry.path().filename().string());
        SetCellValue(1, 0, wxString() << entry.file_size());
    }
};

class InfoWindow : public wxPanel {
   public:
    InfoWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    
    void OnSize(wxSizeEvent& event);

    Grid *grid;
};

#endif // _INFOWINDOW_HPP_