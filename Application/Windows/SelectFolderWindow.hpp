/**
 * @file SelectFolderWindow.hpp
 * @author André Lucas Maegima
 * @brief SelectFolderWindow class definition
 * @version 0.3
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _SELECTFOLDERWINDOW_HPP_
#define _SELECTFOLDERWINDOW_HPP_

#include <wx/wx.h>
#include <wx/listbox.h>
#include <list>
#include "Controllers/Configuration.hpp"
#include "CardPanel.hpp"

class SelectFolderWindow : public wxFrame {
   public:
    SelectFolderWindow(const wxString &title, CardPanel *card);

    void OnNew(wxCommandEvent &event);
    void OnRename(wxCommandEvent &event);
    void OnMove(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);

   private:
    wxListBox *listbox;
    std::list<std::string> ignore_folders;
    wxPanel *CreateBtnPanel(wxPanel *parent);
    CardPanel *card;
};

#endif  // _SELECTFOLDERWINDOW_HPP_