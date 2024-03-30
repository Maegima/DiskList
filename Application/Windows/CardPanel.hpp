/**
 * @file CardPanel.hpp
 * @author André Lucas Maegima
 * @brief CardPanel class definition
 * @version 0.3
 * @date 2024-03-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _CARDPANEL_HPP_
#define _CARDPANEL_HPP_

#include <wx/wx.h>
#include <filesystem>
#include <list>
#include "Controllers/Image.hpp"
#include "Controllers/FileInfo.hpp"

class ListingWindow;

enum {
    MOVE_TO_ROOT = 2500,
    FOLDER_UNWIND = 2501,
    FOLDER_ORGANIZE = 2502,
    DELETE_EMPTY_FOLDERS = 2503,
    MOVE_TO_FOLDER = 2504,
};

class CardPanel : public wxPanel {
   public:
    ListingWindow* parent;
    FileInfo file;
    Image* image;
    wxStaticText* label;
    bool m_mouseInside;
    bool selected;

    CardPanel(ListingWindow* parent, std::filesystem::directory_entry entry, wxString path = "");
    ~CardPanel();

    void OnFolderLeftClick(wxMouseEvent& event);
    void OnFileLeftClick(wxMouseEvent& event);

    void OnEnterPanel(wxMouseEvent& event);
    void OnLeavePanel(wxMouseEvent& event);

    void OnMenuClick(wxCommandEvent& event);
    bool MenuEvent(wxCommandEvent& event, const FileInfo& file, const std::filesystem::path path);
    void OnRightClick(wxMouseEvent& event);
    void OnLeftClick(wxMouseEvent& event);
    void OnTextClick(wxMouseEvent& event);

    struct CompareCards {
        bool operator()(const CardPanel* c1, const CardPanel* c2) const;
    };

    typedef std::list<CardPanel *>::iterator CardIterator;

    void SelectItem(bool select, bool highlight = true);
   private:
    wxStaticText* CreateLabel(std::filesystem::directory_entry entry, wxString path);
    Image* CreateImage(std::filesystem::directory_entry entry);
    std::pair<CardIterator, CardIterator> GetIterators(CardPanel* c1, CardPanel* c2);
};

#endif  // _CARDPANEL_HPP_