/**
 * @file CardPanel.cpp
 * @author André Lucas Maegima
 * @brief CardPanel class implementation
 * @version 0.4
 * @date 2024-04-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "CardPanel.hpp"
#include "ListingWindow.hpp"
#include <wx/utils.h>

CardPanel::CardPanel(ListingWindow *parent, std::filesystem::directory_entry entry, wxString path)
    : wxPanel(parent, wxID_ANY),
      parent(parent),
      file(FileInfo(entry, false)),
      name(path != "" ? path.ToStdString() : entry.path().filename().string()),
      image(CreateImage(entry)),
      label(CreateLabel(entry, path)),
      m_mouseInside(false),
      selected(false),
      to_remove(false) {
    auto sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    sizer->AddSpacer(5);
    sizer->Add(image, 0);
    sizer->Add(label, 0, wxALIGN_CENTER);
    sizer->AddSpacer(10);
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
    Bind(wxEVT_ENTER_WINDOW, &CardPanel::OnEnterPanel, this);
    Bind(wxEVT_LEAVE_WINDOW, &CardPanel::OnLeavePanel, this);
}

CardPanel::~CardPanel() {
    delete image;
    delete label;
}

wxStaticText *CardPanel::CreateLabel(std::filesystem::directory_entry entry, wxString path) {
    wxString name = path.IsEmpty() ? wxString::FromUTF8(entry.path().filename()) : path;
    wxStaticText *text = new wxStaticText(this, wxID_ANY, name, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    int max_text_size = 180;
    while (text->m_width > 200) {
        int parts = text->m_width / max_text_size;
        int size = name.length() / (parts + 1);
        wxString newstr = "";
        for (int i = 0; i < parts; i++) {
            newstr += name.substr(i * size, size) + "\n";
        }
        newstr += name.substr(parts * size);
        text->SetLabel(newstr);
        max_text_size -= 10;
    }
    if (entry.is_directory()) {
        text->Bind(wxEVT_LEFT_DCLICK, &CardPanel::OnFolderLeftClick, this, wxID_ANY);
    } else {
        text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileLeftClick, this, wxID_ANY);
    }
    text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnTextClick, this, wxID_ANY);
    text->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnRightClick, this, wxID_ANY);
    return text;
}

Image *CardPanel::CreateImage(std::filesystem::directory_entry entry) {
    std::vector<std::string> img_exts = parent->config.image_extension;
    std::string extension = entry.path().extension().string();
    Image *img = new Image(this, entry, parent->config);
    if (entry.is_directory()) {
        img->Bind(wxEVT_LEFT_DCLICK, &CardPanel::OnFolderLeftClick, this, wxID_ANY);
    } else {
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileLeftClick, this, wxID_ANY);
    }
    img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnLeftClick, this, wxID_ANY);
    img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnRightClick, this, wxID_ANY);
    return img;
}

std::pair<CardPanel::CardIterator, CardPanel::CardIterator> CardPanel::GetIterators(CardPanel *c1, CardPanel *c2) {
    CardIterator it = this->parent->cards.begin();
    CardIterator first = this->parent->cards.end();
    CardIterator second = this->parent->cards.end();
    while (it != this->parent->cards.end() && first == this->parent->cards.end()) {
        CardPanel *card = *(it++);
        if (card == c1 || card == c2) first = it;
    }
    while (it != this->parent->cards.end() && second == this->parent->cards.end()) {
        CardPanel *card = *(it++);
        if (card == c1 || card == c2) second = it;
    }
    return {first, second};
}

void CardPanel::OnLeftClick(wxMouseEvent &event) {
    if (wxGetKeyState(WXK_CONTROL)) {
        SelectItem(!this->selected);
    } else if (wxGetKeyState(WXK_SHIFT)) {
        CardPanel *last_card = this->parent->selected_card;
        if (last_card != nullptr) {
            auto [begin, end] = GetIterators(last_card, this);
            for (auto it = begin; it != end; it++) {
                (*it)->SelectItem(true);
            }
        }
        SelectItem(true);
    } else {
        for (auto const &card : this->parent->cards) {
            if (card->file.type == FileType::File) {
                card->SelectItem(false, false);
            }
        }
        SelectItem(true);
    }
    this->parent->selected_card = this;
    event.Skip();
}

void CardPanel::SelectItem(bool select, bool highlight) {
    if (this->selected == select) {
        return;
    }
    this->selected = select;
    if (this->selected) {
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_GRAYTEXT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    } else if (highlight) {
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    } else {
        this->label->SetBackgroundColour(*wxWHITE);
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_BTNTEXT));
    }
    if (this->file.type == FileType::Directory) {
        this->parent->selected_folders += this->selected ? 1 : -1;
    } else {
        this->parent->selected_files += this->selected ? 1 : -1;
    }
}

void CardPanel::OnTextClick(wxMouseEvent &event) {
    event.Skip();
}

void CardPanel::OnFolderLeftClick(wxMouseEvent &event) {
    parent->forward_paths.clear();
    parent->ChangePath(file.path);
}

void CardPanel::OnFileLeftClick(wxMouseEvent &event) {
    std::list<std::pair<wxString, wxString>> list;
    list.push_back({"Name", wxString::FromUTF8(file.path.filename().string())});
    list.push_back({"Size", file.size_str()});
    list.push_back({"Created", file.created_str()});
    list.push_back({"Modified", file.modified_str()});
    list.push_back({"Accessed", file.accessed_str()});
    parent->iwindow->FillGrid(list);
}

bool CardPanel::CompareCards::operator()(const CardPanel *c1, const CardPanel *c2) const {
    if (c1->file.type != c2->file.type)
        return c1->file.type == FileType::Directory;
    return c1->name < c2->name;
}

void CardPanel::OnRightClick(wxMouseEvent &evt) {
    wxMenu menu;
    SelectItem(true);
    if (parent->selected_folders > 0 && parent->selected_files == 0) {
        menu.Append(FOLDER_UNWIND, "Unwind files...");
        menu.Append(FOLDER_ORGANIZE, "Organize files...");
        menu.Append(DELETE_EMPTY_FOLDERS, "Delete empty folders...");
    }
    wxMenu *moveMenu = new wxMenu();
    for (int i = MOVE_TO_FOLDER + 1; i < MOVE_TO_FOLDER_MAX; i++) {
        std::string name = parent->last_folders[i].filename();
        if(name != "")
            moveMenu->Append(i, wxString::FromUTF8(name));
    }
    moveMenu->Append(MOVE_TO_FOLDER, "Search...");
    moveMenu->AppendSeparator();
    moveMenu->Append(MOVE_TO_ROOT, "root");
    for (auto event : this->parent->config.folder) {
        moveMenu->Append(event.first, event.second.second);
    }
    menu.AppendSubMenu(moveMenu, "Move to...");
    moveMenu->Connect(wxEVT_MENU, wxCommandEventHandler(ListingWindow::OnCardMenuClick), nullptr, this->parent);
    menu.Connect(wxEVT_MENU, wxCommandEventHandler(ListingWindow::OnCardMenuClick), nullptr, this->parent);
    PopupMenu(&menu);
}

bool CheckPosition(wxRect rect, wxPoint pos, int box) {
    return rect.GetX() < pos.x - box && rect.GetRight() > pos.x + box && rect.GetY() < pos.y - box && rect.GetBottom() > pos.y + box;
}

void CardPanel::OnEnterPanel(wxMouseEvent &event) {
    this->image->ChangeLightness(130);
    this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHT));
    this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    if (selected) {
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_GRAYTEXT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    }
}

void CardPanel::OnLeavePanel(wxMouseEvent &event) {
    auto mousePosition = ClientToScreen(event.GetPosition());
    auto rect = GetScreenRect();
    if (CheckPosition(rect, mousePosition, 0)) {
        this->image->ChangeLightness(130);
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    } else {
        this->image->ChangeLightness(100);
        this->label->SetBackgroundColour(*wxWHITE);
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_BTNTEXT));
    }
    if (selected) {
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_GRAYTEXT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    }
}