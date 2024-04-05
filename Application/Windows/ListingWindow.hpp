/**
 * @file ListingWindow.hpp
 * @author André Lucas Maegima
 * @brief Files listing window
 * @version 0.4
 * @date 2024-04-05
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
    ListingWindow();
    wxScrolledWindow* CreateListingPanel();

    void OnSize(wxSizeEvent& event);
    void OnFolderRightClick(wxMouseEvent& event);
    void OnFolderMenuClick(wxCommandEvent& event);
    void OnKeyPress(wxKeyEvent& event);
    void OnCardMenuClick(wxCommandEvent& event);
    void OnBreadCrumbClick(wxCommandEvent& event);

    void ChangePath(std::filesystem::path path);
    void RefreshPath(bool reload = true);

    void ExecuteMenuEvent(int eventId);
    bool ExecuteCardEvent(int eventId, CardPanel* card, const std::filesystem::path path);

    wxBoxSizer* CreateToolbarSizer();
    wxBoxSizer* CreateWindowSizer();

    FileSystem::Result Move(CardPanel* card, std::filesystem::path path);

    std::list<CardPanel*> cards;
    std::filesystem::path current;
    InfoWindow* iwindow;
    std::map<int, std::filesystem::path> last_folders;
    Configuration config;
    int selected_folders;
    int selected_files;
    CardPanel* selected_card;
    wxScrolledWindow* listing;
    wxBitmapButton* forward;
    wxBitmapButton* backward;
    wxBoxSizer* breadcrumbs;
    std::list<std::string> forward_paths;

   private:
    CardPanel* AddNewCard(std::filesystem::directory_entry entry);
    void OnBackward(wxEvent& event);
    void OnForward(wxEvent& event);
    void UpdatePathBreadCrumbs();
    wxButton* CreateBreadCrumbItem(wxString label, bool enabled = true);
};
#endif  // _LISTINGWINDOW_HPP_