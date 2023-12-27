/**
 * @file CardPanel.cpp
 * @author André Lucas Maegima
 * @brief CardPanel class implementation
 * @version 0.3
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "CardPanel.hpp"
#include "ListingWindow.hpp"
#include "Controllers/FileSystem.hpp"

std::map<std::string, wxImage *> CardPanel::default_images;

void CardPanel::InitializeDefaultIcons(std::map<std::string, std::string> config) {
    for (auto &[key, value] : config) {
        default_images.insert({key, new wxImage(value, wxBITMAP_TYPE_PNG)});
    }
}

CardPanel::CardPanel(ListingWindow *parent, std::filesystem::directory_entry entry, wxString path)
    : wxPanel(parent, wxID_ANY),
      parent(parent),
      file(FileInfo(entry, false)),
      image(CreateImage(entry)),
      label(CreateLabel(entry, path)),
      m_mouseInside(false),
      selected(false) {
    auto sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    sizer->AddSpacer(5);
    sizer->Add(image, 0);
    sizer->Add(label, 0, wxALIGN_CENTER);
    sizer->AddSpacer(10);

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
        text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFolderLeftClick, this, wxID_ANY);
        text->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFolderRightClick, this, wxID_ANY);
    } else {
        text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileLeftClick, this, wxID_ANY);
        text->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFileRightClick, this, wxID_ANY);
    }
    text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnTextClick, this, wxID_ANY);
    return text;
}

Image *CardPanel::CreateImage(std::filesystem::directory_entry entry) {
    Image *img = nullptr;
    std::vector<std::string> img_exts = parent->config.image_extension;
    std::string extension = entry.path().extension().string();
    if (entry.is_directory()) {
        img = new Image(this, default_images["folder"]);
        img->Bind(wxEVT_LEFT_DCLICK, &CardPanel::OnFolderLeftClick, this, wxID_ANY);
        img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFolderRightClick, this, wxID_ANY);
    } else if (std::find(img_exts.begin(), img_exts.end(), extension) != img_exts.end()) {
        wxString path = wxString::FromUTF8(entry.path());
        img = new Image(this, new wxImage(path, wxBITMAP_TYPE_ANY), Image::Type::DYNAMIC);
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileLeftClick, this, wxID_ANY);
        img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFileRightClick, this, wxID_ANY);
    } else if (default_images.contains(extension)) {
        img = new Image(this, default_images[extension]);
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileLeftClick, this, wxID_ANY);
        img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFileRightClick, this, wxID_ANY);
    } else {
        img = new Image(this, default_images["default"]);
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileLeftClick, this, wxID_ANY);
        img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFileRightClick, this, wxID_ANY);
    }
    img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnLeftClick, this, wxID_ANY);
    return img;
}

void CardPanel::OnLeftClick(wxMouseEvent &event) {
    this->selected = !this->selected;
    if (selected) {
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_GRAYTEXT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    } else {
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    }
}

void CardPanel::OnTextClick(wxMouseEvent &event) {
    event.Skip();
}

void CardPanel::OnFolderLeftClick(wxMouseEvent &event) {
    parent->ChangePath(file.path);
}

void CardPanel::OnFileLeftClick(wxMouseEvent &event) {
    std::list<std::pair<wxString, wxString>> list;
    list.push_back({"Name", file.path.filename().string()});
    list.push_back({"Size", file.size_str()});
    list.push_back({"Created", file.created_str()});
    list.push_back({"Modified", file.modified_str()});
    list.push_back({"Accessed", file.accessed_str()});
    parent->iwindow->FillGrid(list);
}

void CardPanel::OnFileRightClick(wxMouseEvent &event) {
    wxMenu menu;
    wxMenu *moveMenu = new wxMenu();
    moveMenu->Append(MOVE_TO_ROOT, "root");
    for (auto event : this->parent->config.folder) {
        moveMenu->Append(event.first, event.second.second);
    }
    menu.AppendSubMenu(moveMenu, "Move to...");
    moveMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CardPanel::OnMenuClick), NULL, this);
    this->selected = true;
    PopupMenu(&menu);
}

bool CardPanel::CompareCards::operator()(const CardPanel *c1, const CardPanel *c2) const {
    if (c1->file.type != c2->file.type)
        return c1->file.type == FileType::Directory;
    return c1->file.path < c2->file.path;
}

void CardPanel::OnMenuClick(wxCommandEvent &evt) {
    bool refresh = false;
    for(auto card : this->parent->cards) {
        if(card->selected) {
            refresh |= MenuEvent(evt, card->file);
        }
    }
    if(refresh) {
        parent->RefreshPath();
    }
}

bool CardPanel::MenuEvent(wxCommandEvent &evt, const FileInfo &file) {
    FileSystem::Result result;
    int eventId = evt.GetId();
    bool refresh = false;
    switch (eventId) {
        case MOVE_TO_ROOT:
            result = FileSystem::Move(file.path, this->parent->config.config["root"]);
            refresh = true;
            break;
        case FOLDER_UNWIND:
            result = FileSystem::UnwindFolder(file.path);
            refresh = true;
            break;
        case FOLDER_ORGANIZE:
            result = FileSystem::OrganizeFolder(file.path, this->parent->config);
            break;
        case DELETE_EMPTY_FOLDERS:
            result = FileSystem::DeleteEmptyFolders(file.path);
            break;
        default:
            if (eventId > 2000 && eventId < 2500) {
                if (this->parent->config.folder.contains(eventId)) {
                    std::filesystem::path folder = this->parent->config.folder[eventId].first;
                    result = FileSystem::Move(file.path, file.path.parent_path() / folder);
                }
                refresh = true;
            }
    }
    if (!result) {
        for (auto const &error : result.errors) {
            wxLogWarning(wxString(error));
        }
    }
    return refresh;
}

void CardPanel::OnFolderRightClick(wxMouseEvent &evt) {
    wxMenu menu;
    menu.Append(FOLDER_UNWIND, "Unwind files...");
    menu.Append(FOLDER_ORGANIZE, "Organize files...");
    menu.Append(DELETE_EMPTY_FOLDERS, "Delete empty folders...");
    wxMenu *moveMenu = new wxMenu();
    moveMenu->Append(MOVE_TO_ROOT, "root");
    for (auto event : this->parent->config.folder) {
        moveMenu->Append(event.first, event.second.second);
    }
    menu.AppendSubMenu(moveMenu, "Move to...");
    moveMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CardPanel::OnMenuClick), NULL, this);
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CardPanel::OnMenuClick), NULL, this);
    this->selected = true;
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