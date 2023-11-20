/**
 * @file ListingWindow.hpp
 * @author André Lucas Maegima
 * @brief Files listing window
 * @version 0.3
 * @date 2023-11-19
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

class ListingWindow : public wxScrolledWindow {
   public:
    ListingWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
    void OnSize(wxSizeEvent& event);
    void ChangePath(std::filesystem::path path);

    std::set<CardPanel*, CardPanel::CompareCards> cards;
    std::filesystem::path current;
};
#endif // _LISTINGWINDOW_HPP_