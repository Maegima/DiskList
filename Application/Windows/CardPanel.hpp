/**
 * @file CardPanel.hpp
 * @author André Lucas Maegima
 * @brief CardPanel class definition
 * @version 0.3
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _CARDPANEL_HPP_
#define _CARDPANEL_HPP_

#include <wx/wx.h>
#include <filesystem>
#include <map>
#include "Controllers/Image.hpp"
#include "Controllers/FileInfo.hpp"

class ListingWindow;

enum {
    FOLDER_UNWIND = 2500,
    FOLDER_ORGANIZE = 2501
};

class CardPanel : public wxPanel {
   public:
    ListingWindow* parent;
    FileInfo file;
    Image* image;
    wxStaticText* label;

    CardPanel(ListingWindow* parent, std::filesystem::directory_entry entry, wxString path = "");
    ~CardPanel();

    void OnFolderLeftClick(wxMouseEvent& event);
    void OnFolderRightClick(wxMouseEvent& event);
    void OnFolderMenuClick(wxCommandEvent& event);

    void OnFileClick(wxMouseEvent& event);
    void OnTextClick(wxMouseEvent& event);

    static void InitializeDefaultIcons(std::map<std::string, std::string> config);

    struct CompareCards {
        bool operator()(const CardPanel* c1, const CardPanel* c2) const;
    };

   private:
    static std::map<std::string, wxImage*> default_images;
    wxStaticText* CreateLabel(std::filesystem::directory_entry entry, wxString path);
    Image* CreateImage(std::filesystem::directory_entry entry);
};

#endif  // _CARDPANEL_HPP_