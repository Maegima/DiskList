/**
 * @file SelectFolderWindow.hpp
 * @author André Lucas Maegima
 * @brief SelectFolderWindow class definition
 * @version 0.3
 * @date 2024-03-19
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

class SelectFolderWindow : public wxFrame {
   public:
    SelectFolderWindow(const wxString &title, Configuration &config);

    void OnNew(wxCommandEvent &event);
    void OnRename(wxCommandEvent &event);
    void OnClear(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);

   private:
    wxListBox *listbox;
    std::list<std::string> ignore_folders;
    wxPanel *CreateBtnPanel(wxPanel *parent);
};

#endif  // _SELECTFOLDERWINDOW_HPP_