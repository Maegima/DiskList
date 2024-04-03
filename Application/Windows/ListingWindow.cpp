/**
 * @file ListingWindow.cpp
 * @author André Lucas Maegima
 * @brief Listing window implementation
 * @version 0.4
 * @date 2024-04-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "wx/wrapsizer.h"
#include "ListingWindow.hpp"
#include "Controllers/Algorithm.hpp"

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

void ListingWindow::RefreshPath(bool reload) {
    auto* sizer = this->GetSizer();
    sizer->Clear(reload);
    this->selected_files = 0;
    this->selected_folders = 0;
    this->SetFocus();
    if(reload) {
        this->cards.clear();
        for (auto const& entry : std::filesystem::directory_iterator{current}) {
            AddNewCard(entry);
        }
        if (this->current.has_parent_path() && this->current != this->config.config["root"]) {
            AddNewCard(std::filesystem::directory_entry(this->current.parent_path()), "..");
        }
        cards.sort(CardPanel::CompareCards());
    } else {
        std::list<CardPanel*> erase_list;
        for (auto const &card : cards) {
            if(card->to_remove) {
                erase_list.push_back(card);
            }
        }
        std::erase_if(cards, [](CardPanel *card) { return card->to_remove; });
        for(auto card : erase_list) {
            delete card;
        }
    }
    for (auto const& card : cards) {
        sizer->Add(card, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 0);
    }
    //this->SetScrollbars(0, 40, 0, sizer->GetSize().GetHeight() / 40);
    this->SendSizeEvent();
    this->Refresh();
}

void ListingWindow::OnCardMenuClick(wxCommandEvent &evt) {
    ExecuteMenuEvent(evt.GetId());
}

void ListingWindow::ExecuteMenuEvent(int eventId) {
    bool refresh = false;
    wxDirDialog *lsw = nullptr;
    std::filesystem::path path = this->config.config["root"];
    switch (eventId) {
        case MOVE_TO_FOLDER:
            lsw = new wxDirDialog(this, "Select folder:", path.string());
            lsw->ShowModal();
            path = lsw->GetPath().ToUTF8().data();
            for(int i = MOVE_TO_FOLDER_MAX - 2; i > MOVE_TO_FOLDER; i--){
                last_folders[i + 1] = last_folders[i];
            }
            last_folders[MOVE_TO_FOLDER + 1] = path;
            break;
    }
    for (auto card : this->cards) {
        if (card->selected) {
            refresh |= ExecuteCardEvent(eventId, card, path);
        }
    }
    RefreshPath(refresh);
}

bool ListingWindow::ExecuteCardEvent(int eventId, CardPanel* card, const std::filesystem::path path) {
    FileSystem::Result result;
    bool refresh = false;
    switch (eventId) {
        case MOVE_TO_FOLDER:
        case MOVE_TO_ROOT:
            result = this->Move(card, path);
            break;
        case FOLDER_UNWIND:
            result = FileSystem::UnwindFolder(card->file.path);
            refresh = true;
            break;
        case FOLDER_ORGANIZE:
            result = FileSystem::OrganizeFolder(card->file.path, this->config);
            break;
        case DELETE_EMPTY_FOLDERS:
            result = FileSystem::DeleteEmptyFolders(card->file.path);
            break;
        default:
            if (eventId > 2000 && eventId < 2500 && this->config.folder.contains(eventId)) {
                std::filesystem::path folder = this->config.folder[eventId].first;
                result = this->Move(card, card->file.path.parent_path() / folder);
            } else if (eventId > MOVE_TO_FOLDER && eventId < MOVE_TO_FOLDER_MAX) {
                result = this->Move(card, last_folders[eventId]);
            }
    }
    if (!result) {
        for (auto const &error : result.errors) {
            wxLogWarning(wxString(error));
        }
    }
    return refresh;
}

FileSystem::Result ListingWindow::Move(CardPanel* card, std::filesystem::path path) {
    auto result = FileSystem::Move(card->file.path, path);
    for(const auto &item : result.created) {
        if(item.parent_path() == current) {
            AddNewCard(std::filesystem::directory_entry(item));
            cards.sort(CardPanel::CompareCards());
        }
    }
    if(result) {
        card->to_remove = true;
    }
    return result;
}

CardPanel* ListingWindow::AddNewCard(std::filesystem::directory_entry entry, std::string label) {
    auto card = new CardPanel(this, entry, label);
    card->Bind(wxEVT_RIGHT_DOWN, &ListingWindow::OnFolderRightClick, this, wxID_ANY);
    cards.push_back(card);
    return card;
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
    int event_first = 2001;
    int key = '1';
    int max_shortcut = 9;
    if(event.ControlDown()) {
        for(int i = 0; i < max_shortcut; i++) {
            int eventId = event_first + i;
            int currenKey = key + i;
            if(uc == currenKey && this->config.folder.contains(eventId)) {
                ExecuteMenuEvent(eventId);
            }
        }
    }
    event.Skip();
}

void ListingWindow::OnFolderRightClick(wxMouseEvent& evt) {
    wxMenu menu;
    menu.Append(FOLDER_ORGANIZE, "Organize folder...");
    menu.Append(DELETE_EMPTY_FOLDERS, "Delete empty folders...");
    menu.Connect(wxEVT_MENU, wxCommandEventHandler(ListingWindow::OnFolderMenuClick), nullptr, this);
    PopupMenu(&menu);
}
