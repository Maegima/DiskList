/**
 * @file SelectFolderWindow.hpp
 * @author André Lucas Maegima
 * @brief SelectFolderWindow class implementation
 * @version 0.3
 * @date 2024-03-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "SelectFolderWindow.hpp"
#include <wx/textdlg.h>
#include <filesystem>
#include <algorithm>

SelectFolderWindow::SelectFolderWindow(const wxString& title, Configuration &config) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 350)) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    for(auto const& item :config.folder) {
        ignore_folders.push_back(item.second.first);
    }
    
    listbox = new wxListBox(panel, ID_LISTBOX, wxPoint(-1, -1), wxSize(-1, -1));
    for (auto const& entry : std::filesystem::directory_iterator{config.config["root"]}) {
        if(entry.is_directory()) {
            std::string filename = entry.path().filename().string();
            if(std::find(ignore_folders.begin(), ignore_folders.end(), filename) == ignore_folders.end()){
                listbox->Append(filename);
            }
        }
    }

    vbox->Add(listbox, 1, wxEXPAND | wxALL, 20);

    wxPanel *btn_panel = CreateBtnPanel(panel);
    vbox->Add(btn_panel, 0, wxEXPAND | wxRIGHT, 20);
    vbox->Add(-1, 20);

    Bind(wxEVT_LISTBOX_DCLICK, &SelectFolderWindow::OnRename, this);

    panel->SetSizer(vbox);
    Center();
}

wxPanel* SelectFolderWindow::CreateBtnPanel(wxPanel *parent) {
    wxPanel* btnPanel = new wxPanel(parent, wxID_ANY);

    wxButton* newb = new wxButton(btnPanel, wxID_NEW, "New");
    wxButton* renameb = new wxButton(btnPanel, wxID_EDIT, "Edit");
    wxButton* deleteb = new wxButton(btnPanel, wxID_DELETE, "Delete");
    wxButton* clearb = new wxButton(btnPanel, wxID_CLEAR, "Clear");

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(20, -1);
    hbox->Add(newb, 0, wxTOP | wxLEFT, 5);
    hbox->Add(renameb, 0, wxTOP | wxLEFT, 5);
    hbox->Add(deleteb, 0, wxTOP | wxLEFT, 5);
    hbox->Add(clearb, 0, wxTOP | wxLEFT, 5);
    
    btnPanel->Bind(wxEVT_BUTTON, &SelectFolderWindow::OnNew, this, wxID_NEW);
    btnPanel->Bind(wxEVT_BUTTON, &SelectFolderWindow::OnRename, this, wxID_EDIT);
    btnPanel->Bind(wxEVT_BUTTON, &SelectFolderWindow::OnClear, this, wxID_CLEAR);
    btnPanel->Bind(wxEVT_BUTTON, &SelectFolderWindow::OnDelete, this, wxID_DELETE);

    btnPanel->SetSizer(hbox);
    return btnPanel;
}

void SelectFolderWindow::OnNew(wxCommandEvent& event) {
    wxString str = wxGetTextFromUser("Add new item");
    if (str.Len() > 0)
        listbox->Append(str);
}

void SelectFolderWindow::OnRename(wxCommandEvent& event) {
    wxString text;
    wxString renamed;

    int sel = listbox->GetSelection();
    if (sel != -1) {
        text = listbox->GetString(sel);
        renamed = wxGetTextFromUser("Rename item", "Rename dialog", text);
    }

    if (!renamed.IsEmpty()) {
        listbox->Delete(sel);
        listbox->Insert(renamed, sel);
    }
}

void SelectFolderWindow::OnClear(wxCommandEvent& event) {
    listbox->Clear();
}

void SelectFolderWindow::OnDelete(wxCommandEvent& event) {
    int sel = listbox->GetSelection();
    if (sel != -1) {
        listbox->Delete(sel);
    }
}