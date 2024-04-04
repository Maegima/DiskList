/**
 * @file CardPanel.hpp
 * @author André Lucas Maegima
 * @brief CardPanel class definition
 * @version 0.3
 * @date 2024-04-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _CARDPANEL_HPP_
#define _CARDPANEL_HPP_

#include <wx/wx.h>
#include <list>
#include "Controllers/Image.hpp"
#include "Controllers/FileInfo.hpp"
#include "Controllers/FileSystem.hpp"

class ListingWindow;

enum {
    MOVE_TO_ROOT = 2500,
    FOLDER_UNWIND = 2501,
    FOLDER_ORGANIZE = 2502,
    DELETE_EMPTY_FOLDERS = 2503,
    MOVE_TO_FOLDER = 2504,
    MOVE_TO_FOLDER_MAX = 2508
};

class CardPanel : public wxPanel {
   public:
    ListingWindow* parent;
    FileInfo file;
    std::string name;
    Image* image;
    wxStaticText* label;
    bool m_mouseInside;
    bool selected;
    bool to_remove;

    CardPanel(ListingWindow* parent, std::filesystem::directory_entry entry);
    ~CardPanel();

    void OnFolderLeftClick(wxMouseEvent& event);
    void OnFileLeftClick(wxMouseEvent& event);

    void OnEnterPanel(wxMouseEvent& event);
    void OnLeavePanel(wxMouseEvent& event);

    void OnRightClick(wxMouseEvent& event);
    void OnLeftClick(wxMouseEvent& event);
    void OnTextClick(wxMouseEvent& event);

    struct CompareCards {
        bool operator()(const CardPanel* c1, const CardPanel* c2) const;
    };

    typedef std::list<CardPanel *>::iterator CardIterator;

    void SelectItem(bool select, bool highlight = true);
   private:
    wxStaticText* CreateLabel(std::filesystem::directory_entry entry);
    Image* CreateImage(std::filesystem::directory_entry entry);
    std::pair<CardIterator, CardIterator> GetIterators(CardPanel* c1, CardPanel* c2);
};

#endif  // _CARDPANEL_HPP_