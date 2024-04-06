
/*
 * @file InfoWindow.hpp
 * @author André Lucas Maegima
 * @brief Information Window class definition
 * @version 0.4
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
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

    void FillGrid(std::list<std::pair<wxString, wxString>> lines);\
   private:
    wxPanel* CreateCenteredText(wxString label, wxSize size = wxDefaultSize);
};

#endif  // _INFOWINDOW_HPP_