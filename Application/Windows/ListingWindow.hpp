/**
 * @file ListingWindow.hpp
 * @author André Lucas Maegima
 * @brief Files listing window
 * @version 0.4
 * @date 2024-04-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _LISTINGWINDOW_HPP_
#define _LISTINGWINDOW_HPP_

#include <wx/wx.h>
#include <map>
#include "CardPanel.hpp"
#include "InfoWindow.hpp"
#include "Controllers/FileSystem.hpp"
#include "Controllers/Configuration.hpp"

class ListingWindow : public wxScrolledWindow {
   public:
    ListingWindow(wxWindow* parent, InfoWindow* iwindow, wxWindowID id, const wxPoint& pos, const wxSize& size);
    void OnSize(wxSizeEvent& event);
    void OnFolderRightClick(wxMouseEvent& event);
    void OnFolderMenuClick(wxCommandEvent& event);
    void OnKeyPress(wxKeyEvent& event);
    void OnCardMenuClick(wxCommandEvent& event);

    void ChangePath(std::filesystem::path path);
    void RefreshPath(bool reload = true);

    void ExecuteMenuEvent(int eventId);
    bool ExecuteCardEvent(int eventId, CardPanel *card, const std::filesystem::path path);

    FileSystem::Result Move(CardPanel* card, std::filesystem::path path);

    std::list<CardPanel*> cards;
    std::filesystem::path current;
    InfoWindow* iwindow;
    std::map<int, std::filesystem::path> last_folders;
    Configuration config;
    int selected_folders;
    int selected_files;
    CardPanel* selected_card;

   private:
    CardPanel* AddNewCard(std::filesystem::directory_entry entry, std::string label = "");
};
#endif  // _LISTINGWINDOW_HPP_