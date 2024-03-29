/**
 * @file ListingWindow.cpp
 * @author André Lucas Maegima
 * @brief Listing window implementation
 * @version 0.3
 * @date 2024-03-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "wx/wrapsizer.h"
#include "ListingWindow.hpp"
#include "Controllers/FileSystem.hpp"

ListingWindow::ListingWindow(wxWindow* parent, InfoWindow* iwindow, wxWindowID id, const wxPoint& pos, const wxSize& size)
    : wxScrolledWindow(parent, id, pos, size, wxSUNKEN_BORDER),
      iwindow(iwindow),
      config(".conf"),
      selected_folders(0),
      selected_files(0),
      selected_card(nullptr) {
    SetBackgroundColour(*wxWHITE);

    Bind(wxEVT_SIZE, &ListingWindow::OnSize, this, wxID_ANY);
    Bind(wxEVT_RIGHT_DOWN, &ListingWindow::OnFolderRightClick, this, wxID_ANY);
    Bind(wxEVT_CHAR_HOOK, &ListingWindow::OnKeyPress, this, wxID_ANY);

    wxWrapSizer* sizer = new wxWrapSizer(wxHORIZONTAL);
    SetSizer(sizer);
    ChangePath(config.config["root"]);

    this->SetScrollbars(0, 40, 0, sizer->GetSize().GetHeight() / 40);
}

void ListingWindow::OnSize(wxSizeEvent& event) {
    int width = this->m_width;
    int items = width / 200;
    int spacing = items ? (width % 200) / items : 0;
    auto sizer = GetSizer();
    for (auto& item : sizer->GetChildren()) {
        item->SetBorder(spacing / 2);
    }
    auto newSize = this->m_parent->GetClientSize();
    this->SetSize(newSize.x - 250, newSize.y);
    Refresh();
    event.Skip();
}

void ListingWindow::ChangePath(std::filesystem::path path) {
    this->current = path;
    RefreshPath();
}

void ListingWindow::RefreshPath() {
    auto* sizer = this->GetSizer();
    sizer->Clear(true);
    this->cards.clear();
    this->selected_files = 0;
    this->selected_folders = 0;
    if (this->current.has_parent_path() && this->current != this->config.config["root"]) {
        std::filesystem::directory_entry parent_entry(this->current.parent_path());
        auto card = new CardPanel(this, parent_entry, "..");
        card->Bind(wxEVT_RIGHT_DOWN, &ListingWindow::OnFolderRightClick, this, wxID_ANY);
        cards.insert(card);
    }
    for (auto const& entry : std::filesystem::directory_iterator{current}) {
        auto card = new CardPanel(this, entry);
        card->Bind(wxEVT_RIGHT_DOWN, &ListingWindow::OnFolderRightClick, this, wxID_ANY);
        cards.insert(card);
    }
    for (auto const& card : cards) {
        sizer->Add(card, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 0);
    }
    this->SetScrollbars(0, 40, 0, sizer->GetSize().GetHeight() / 40);
    this->SendSizeEvent();
    this->Refresh();
    this->SetFocus();
}

void ListingWindow::OnFolderMenuClick(wxCommandEvent& evt) {
    FileSystem::Result result;
    switch (evt.GetId()) {
        case FOLDER_ORGANIZE:
            result = FileSystem::OrganizeCurrentFolder(this->current, this->config);
            RefreshPath();
            break;
        case DELETE_EMPTY_FOLDERS:
            result = FileSystem::DeleteEmptyFolders(this->current);
            RefreshPath();
            break;
    }
    if (!result) {
        for (auto const& error : result.errors) {
            wxLogWarning(wxString(error));
        }
    }
}

void ListingWindow::OnKeyPress(wxKeyEvent& event) {
    int uc = event.GetKeyCode();
    if(event.ControlDown() && uc == 'A') {
        for (auto const& card : cards) {
            if(card->file.type == FileType::File) {
                card->SelectItem(true);
            }
        }
    }
}

void ListingWindow::OnFolderRightClick(wxMouseEvent& evt) {
    wxMenu menu;
    menu.Append(FOLDER_ORGANIZE, "Organize folder...");
    menu.Append(DELETE_EMPTY_FOLDERS, "Delete empty folders...");
    menu.Connect(wxEVT_MENU, wxCommandEventHandler(ListingWindow::OnFolderMenuClick), nullptr, this);
    PopupMenu(&menu);
}
