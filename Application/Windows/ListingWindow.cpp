/**
 * @file ListingWindow.cpp
 * @author André Lucas Maegima
 * @brief Listing window implementation
 * @version 0.4
 * @date 2024-04-04
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

    wxImage rightImage = config.image["forward"]->Scale(38, 38, wxIMAGE_QUALITY_HIGH);
    wxImage leftImage = config.image["backward"]->Scale(38, 38, wxIMAGE_QUALITY_HIGH);
    wxImage rightImageHover = rightImage.Copy();
    wxImage leftImageHover = leftImage.Copy();
    leftImageHover.ChangeHSV(1, 1, -0.25);
    rightImageHover.ChangeHSV(1, 1, -0.25);

    forward = new wxBitmapButton(this->GetParent(), wxID_FORWARD, rightImage, wxDefaultPosition, wxSize(32, 32), wxBORDER_NONE);
    backward = new wxBitmapButton(this->GetParent(), wxID_BACKWARD, leftImage, wxDefaultPosition, wxSize(32, 32), wxBORDER_NONE);
    forward->SetBitmapHover(rightImageHover);
    backward->SetBitmapHover(leftImageHover);

    breadcrumbs = new wxBoxSizer(wxHORIZONTAL);

    Bind(wxEVT_SIZE, &ListingWindow::OnSize, this, wxID_ANY);
    Bind(wxEVT_RIGHT_DOWN, &ListingWindow::OnFolderRightClick, this, wxID_ANY);
    Bind(wxEVT_CHAR_HOOK, &ListingWindow::OnKeyPress, this, wxID_ANY);

    backward->Bind(wxEVT_BUTTON, &ListingWindow::OnBackward, this);
    forward->Bind(wxEVT_BUTTON, &ListingWindow::OnForward, this);
    Bind(wxEVT_AUX1_DOWN, &ListingWindow::OnBackward, this);
    Bind(wxEVT_AUX2_DOWN, &ListingWindow::OnForward, this);

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
    std::error_code errorcode;
    if (this->current != path && std::filesystem::is_directory(path, errorcode)) {
        this->current = path;
        backward->Enable(current != config.config["root"]);
        forward->Enable(forward_paths.size() > 0);
        UpdatePathBreadCrumbs();
        RefreshPath();
    }
}

void ListingWindow::OnBackward(wxEvent& event) {
    if (current != this->config.config["root"]) {
        forward_paths.push_front(current);
        ChangePath(current.parent_path());
    }
}

void ListingWindow::OnForward(wxEvent& event) {
    if (forward_paths.size() > 0) {
        std::string path = forward_paths.front();
        forward_paths.pop_front();
        ChangePath(path);
    }
}

void ListingWindow::UpdatePathBreadCrumbs() {
    size_t rootlength = config.config["root"].length();
    std::string display = this->current.string().length() <= rootlength ? "" : this->current.string().substr(rootlength + 1);
    wxArrayString folders = wxSplit(wxString::FromUTF8(display), '/');
    breadcrumbs->Clear(true);
    std::filesystem::path path = config.config["root"];
    for (const auto& folder : folders) {
        path /= folder.ToUTF8().data();
        breadcrumbs->Add(CreateBreadCrumbItem("<tt><b>&gt;</b></tt>", false), 0, wxEXPAND | wxALL, 1);
        auto item = CreateBreadCrumbItem(folder);
        item->SetName(wxString::FromUTF8(path.string()));
        item->Bind(wxEVT_BUTTON, &ListingWindow::OnBreadCrumbClick, this);
        breadcrumbs->Add(item, 0, wxEXPAND | wxALL, 1);
    }
    if (folders.empty()) {
        breadcrumbs->Add(CreateBreadCrumbItem("<tt><b>&gt;</b></tt>", false), 0, wxEXPAND | wxALL, 1);
    }
}

wxButton* ListingWindow::CreateBreadCrumbItem(wxString label, bool enabled) {
    auto item = new wxButton(this->GetParent(), wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxBU_EXACTFIT);
    item->SetLabelMarkup(label);
    item->Enable(enabled);
    return item;
}

void ListingWindow::RefreshPath(bool reload) {
    auto* sizer = this->GetSizer();
    sizer->Clear(reload);
    this->selected_files = 0;
    this->selected_folders = 0;
    this->SetFocus();
    if (reload) {
        this->cards.clear();
        for (auto const& entry : std::filesystem::directory_iterator{current}) {
            AddNewCard(entry);
        }
        cards.sort(CardPanel::CompareCards());
    } else {
        std::list<CardPanel*> erase_list;
        for (auto const& card : cards) {
            if (card->to_remove) {
                erase_list.push_back(card);
            }
        }
        std::erase_if(cards, [](CardPanel* card) { return card->to_remove; });
        for (auto card : erase_list) {
            delete card;
        }
    }
    for (auto const& card : cards) {
        sizer->Add(card, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 0);
    }
    if (reload) {
        this->SetScrollbars(0, 40, 0, sizer->GetSize().GetHeight() / 40);
    }
    this->SendSizeEvent();
    this->GetParent()->SendSizeEvent();
    this->Refresh();
}

void ListingWindow::OnCardMenuClick(wxCommandEvent& evt) {
    ExecuteMenuEvent(evt.GetId());
}

void ListingWindow::OnBreadCrumbClick(wxCommandEvent& event) {
    wxButton* breadcrumb = (wxButton*)event.GetEventObject();
    ChangePath(breadcrumb->GetName().ToUTF8().data());
}

void ListingWindow::ExecuteMenuEvent(int eventId) {
    bool refresh = false;
    wxDirDialog* lsw = nullptr;
    std::filesystem::path path = "";
    std::filesystem::path root = this->config.config["root"];
    if (eventId == MOVE_TO_FOLDER) {
        lsw = new wxDirDialog(this, "Select folder:", root.string());
        lsw->ShowModal();
        path = lsw->GetPath().ToUTF8().data();
        for (int i = MOVE_TO_FOLDER_MAX - 2; i > MOVE_TO_FOLDER; i--) {
            last_folders[i + 1] = last_folders[i];
        }
        last_folders[MOVE_TO_FOLDER + 1] = path;
    } else if (eventId >= MOVE_TO_ROOT && eventId <= DELETE_EMPTY_FOLDERS) {
        path = root;
    } else if (eventId > 2000 && eventId < 2500 && this->config.folder.contains(eventId)) {
        path = current / this->config.folder[eventId].first;
        eventId = MOVE_TO_FOLDER;
    } else if (eventId > MOVE_TO_FOLDER && eventId < MOVE_TO_FOLDER_MAX) {
        path = last_folders[eventId];
        eventId = MOVE_TO_FOLDER;
    }
    if (!path.empty()) {
        for (auto card : this->cards) {
            if (card->selected) {
                refresh |= ExecuteCardEvent(eventId, card, path);
            }
        }
        RefreshPath(refresh);
    }
}

bool ListingWindow::ExecuteCardEvent(int eventId, CardPanel* card, const std::filesystem::path path) {
    FileSystem::Result result;
    bool refresh = false;
    switch (eventId) {
        case MOVE_TO_ROOT:
        case MOVE_TO_FOLDER:
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
            wxLogWarning("Invalid event %d", eventId);
            break;
    }
    if (!result) {
        for (auto const& error : result.errors) {
            wxLogWarning(wxString(error));
        }
    }
    return refresh;
}

FileSystem::Result ListingWindow::Move(CardPanel* card, std::filesystem::path path) {
    auto result = FileSystem::Move(card->file.path, path);
    for (const auto& item : result.created) {
        if (item.parent_path() == current) {
            AddNewCard(std::filesystem::directory_entry(item));
            cards.sort(CardPanel::CompareCards());
        }
    }
    if (result) {
        card->to_remove = true;
    }
    return result;
}

CardPanel* ListingWindow::AddNewCard(std::filesystem::directory_entry entry) {
    auto card = new CardPanel(this, entry);
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
    int event_first = 2001;
    if (event.ControlDown()) {
        switch (uc) {
            case '1' ... '9':
                ExecuteMenuEvent(event_first + uc - '1');
                break;
            case 'A':
                for (auto const& card : cards) {
                    if (card->file.type == FileType::File) {
                        card->SelectItem(true);
                    }
                }
                break;
        }
    } else if (event.AltDown()) {
        switch (uc) {
            case WXK_LEFT:
                OnBackward(event);
                break;
            case WXK_RIGHT:
                OnForward(event);
                break;
        }
    }
    if (uc >= WXK_F6 || uc <= WXK_F7) {
        ExecuteMenuEvent(MOVE_TO_FOLDER + 1 + uc - WXK_F6);
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
