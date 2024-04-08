/**
 * @file ListingWindow.hpp
 * @author André Lucas Maegima
 * @brief Files listing window
 * @version 0.4
 * @date 2024-04-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _LISTINGWINDOW_HPP_
#define _LISTINGWINDOW_HPP_

#include <wx/wx.h>
#include <wx/sizer.h>
#include <map>
#include "CardPanel.hpp"
#include "InfoWindow.hpp"
#include "Controllers/FileSystem.hpp"
#include "Controllers/Configuration.hpp"

class ListingWindow : public wxFrame {
   public:
    Configuration config;
    wxScrolledWindow* lwindow;
    InfoWindow* iwindow;
    wxBoxSizer* breadcrumbs;
    wxBitmapButton* forward;
    wxBitmapButton* backward;
    
    int selected_folders;
    int selected_files;
    CardPanel* selected_card;

    std::filesystem::path current;
    std::list<CardPanel*> folder_cards;
    std::list<CardPanel*> file_cards;
    std::list<std::string> forward_paths;
    std::map<int, std::filesystem::path> last_folders;

    ListingWindow();

    void ChangePath(std::filesystem::path path);
    void ExecuteMenuEvent(int eventId);

   private:
    wxScrolledWindow* CreateListingPanel();
    wxBitmapButton* CreateBitmapButton(wxWindowID id, std::string name);
    wxBoxSizer* CreateSizer();
    wxButton* CreateBreadCrumbItem(wxString label, bool enabled = true);
    CardPanel* CreateCard(std::filesystem::directory_entry entry);
    
    void UpdatePathBreadCrumbs();
    bool ExecuteCardEvent(int eventId, CardPanel* card, const std::filesystem::path path);
    void RefreshPath(bool reload = true);
    void RefreshStatusText();
    FileSystem::Result Move(CardPanel* card, std::filesystem::path path);

    void OnSize(wxSizeEvent& event);
    void OnFolderRightClick(wxMouseEvent& event);
    void OnFolderMenuClick(wxCommandEvent& event);
    void OnKeyPress(wxKeyEvent& event);
    void OnBreadCrumbClick(wxCommandEvent& event);
    void OnBackward(wxEvent& event);
    void OnForward(wxEvent& event);
};
#endif  // _LISTINGWINDOW_HPP_