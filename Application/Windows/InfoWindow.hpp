
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
#include <list>

class InfoWindow : public wxPanel {
   public:
    InfoWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);

    void OnSize(wxSizeEvent& event);

    void FillGrid(std::list<std::pair<wxString, wxString>> lines);

    wxGrid* grid;

   private:
    wxGrid* CreateGrid();
};

#endif  // _INFOWINDOW_HPP_