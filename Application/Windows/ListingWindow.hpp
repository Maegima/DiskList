/**
 * @file ListingWindow.hpp
 * @author André Lucas Maegima
 * @brief Files listing window
 * @version 0.3
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _LISTINGWINDOW_HPP_
#define _LISTINGWINDOW_HPP_

#include <wx/wx.h>
#include <set>
#include <filesystem>
#include "CardPanel.hpp"
#include "InfoWindow.hpp"
#include "Controllers/Configuration.hpp"

class ListingWindow : public wxScrolledWindow {
   public:
    ListingWindow(wxWindow* parent, InfoWindow *iwindow, wxWindowID id, const wxPoint& pos, const wxSize& size);
    void OnSize(wxSizeEvent& event);
    void OnFolderRightClick(wxMouseEvent& event);
    void OnFolderMenuClick(wxCommandEvent& event);
    
    void ChangePath(std::filesystem::path path);
    void RefreshPath();

    std::set<CardPanel*, CardPanel::CompareCards> cards;
    std::filesystem::path current;
    InfoWindow *iwindow;
    Configuration config;
    int selected_folders;
    int selected_files;
};
#endif // _LISTINGWINDOW_HPP_