/**
 * @file CardPanel.cpp
 * @author André Lucas Maegima
 * @brief CardPanel class implementation
 * @version 0.3
 * @date 2023-12-06
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
      label(CreateLabel(entry, path)) {
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
    std::string extension = entry.path().extension().string();
    if (entry.is_directory()) {
        img = new Image(this, default_images["folder"]);
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFolderLeftClick, this, wxID_ANY);
        img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFolderRightClick, this, wxID_ANY);
    } else if (extension == ".png") {
        wxString path = wxString::FromUTF8(entry.path());
        img = new Image(this, new wxImage(path, wxBITMAP_TYPE_PNG), Image::Type::DYNAMIC);
    } else if (default_images.contains(extension)) {
        img = new Image(this, default_images[extension]);
    } else {
        img = new Image(this, default_images["default"]);
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileLeftClick, this, wxID_ANY);
        img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFileRightClick, this, wxID_ANY);
    }
    return img;
}

void CardPanel::OnTextClick(wxMouseEvent &event) {
    std::cout << label->GetLabel() << "\n";
    event.Skip();
}

void CardPanel::OnFolderLeftClick(wxMouseEvent &event) {
    std::cout << label->GetLabel() << " " << parent->current << "\n";
    parent->ChangePath(file.path);
}

void CardPanel::OnFileLeftClick(wxMouseEvent &event) {
    std::cout << label->GetLabel() << " " << parent->current << " file\n";
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
    for (auto event : this->parent->config.folder) {
        menu.Append(event.first, "Move to " + event.second.second);
    }
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CardPanel::OnFileMenuClick), NULL, this);
    PopupMenu(&menu);
}


bool CardPanel::CompareCards::operator()(const CardPanel *c1, const CardPanel *c2) const {
    if (c1->file.type != c2->file.type)
        return c1->file.type == FileType::Directory;
    return c1->file.path < c2->file.path;
}

void CardPanel::OnFolderMenuClick(wxCommandEvent &evt) {
    FileSystem::Result result;
    int eventId = evt.GetId();
    switch (eventId) {
        case FOLDER_UNWIND:
            result = FileSystem::UnwindFolder(this->file.path);
            parent->RefreshPath();
            break;
        case FOLDER_ORGANIZE:
            result = FileSystem::OrganizeFolder(this->file.path, this->parent->config);
            break;
    }
    if(!result) {
        for(auto const &error : result.errors) {
            wxLogWarning(wxString(error));
        }
    }
}

void CardPanel::OnFileMenuClick(wxCommandEvent &evt) {
    int eventId = evt.GetId();
    switch (eventId) {
        default:
            if (eventId > 2000 && eventId < 3000) {
                if (this->parent->config.folder.contains(eventId)) {
                    std::filesystem::path folder = this->parent->config.folder[eventId].first;
                    std::filesystem::path move_path = this->file.path.parent_path() / folder;
                    std::filesystem::path new_path = move_path / this->file.path.filename();
                    if (!std::filesystem::exists(move_path)) {
                        std::filesystem::create_directory(move_path);
                    }
                    if (!std::filesystem::exists(new_path)) {
                        std::filesystem::rename(this->file.path, new_path);
                        parent->RefreshPath();
                    }
                }
            }
    }
}

void CardPanel::OnFolderRightClick(wxMouseEvent &evt) {
    wxMenu menu;
    menu.Append(FOLDER_UNWIND, "Unwind files...");
    menu.Append(FOLDER_ORGANIZE, "Organize files...");
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CardPanel::OnFolderMenuClick), NULL, this);
    PopupMenu(&menu);
}

bool CheckPosition(wxRect rect, wxPoint pos, int box) {
    return rect.GetX() < pos.x - box && rect.GetRight() > pos.x + box && rect.GetY() < pos.y - box && rect.GetBottom() > pos.y + box;
}

void CardPanel::OnEnterPanel(wxMouseEvent &event) {
    this->image->ChangeLightness(130);
    this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHT));
    this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
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
}