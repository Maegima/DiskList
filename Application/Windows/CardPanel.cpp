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
    if(entry.is_directory()){
        text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFolderLeftClick, this, wxID_ANY);
        text->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFolderRightClick, this, wxID_ANY);
    } else {
        text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileClick, this, wxID_ANY);
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
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileClick, this, wxID_ANY);
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

void CardPanel::OnFileClick(wxMouseEvent &event) {
    std::cout << label->GetLabel() << " " << parent->current << " file\n";
    std::list<std::pair<wxString, wxString>> list;
    list.push_back({"Name", file.path.filename().string()});
    list.push_back({"Size", file.size_str()});
    list.push_back({"Created", file.created_str()});
    list.push_back({"Modified", file.modified_str()});
    list.push_back({"Accessed", file.accessed_str()});
    parent->iwindow->FillGrid(list);
}

bool CardPanel::CompareCards::operator()(const CardPanel *c1, const CardPanel *c2) const {
    if (c1->file.type != c2->file.type)
        return c1->file.type == FileType::Directory;
    return c1->file.path < c2->file.path;
}

void entry_move(const std::filesystem::path &entry, const Configuration &config) {
    std::string extension = entry.extension().string();
    std::filesystem::path path = entry.parent_path();
    for (const auto &[key, list] : config.organize) {
        if (std::find(list.begin(), list.end(), extension) != list.end()) {
            path /= key;
            break;
        }
    }
    path /= entry.filename().c_str();
    if (path != entry) {
        if (!std::filesystem::exists(path.parent_path())) {
            std::filesystem::create_directory(path.parent_path());
        }
        std::filesystem::rename(entry, path);
    }
}

void organize_file(const std::filesystem::path &path, const Configuration &config) {
    std::filesystem::path folder = path;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        std::filesystem::path filename = entry.path().filename();
        if (entry.is_regular_file()) {
            try {
                entry_move(entry, config);
            } catch (std::filesystem::filesystem_error &e) {
            }
        } else if (entry.is_directory() && !config.organize.contains(filename)) {
            organize_file(entry, config);
        }
    }
}

std::list<std::filesystem::path> get_files(const std::filesystem::path &path) {
    std::list<std::filesystem::path> files;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        if (entry.is_regular_file()) {
            files.push_back(entry);
        } else if (entry.is_directory()) {
            files.splice(files.end(), get_files(entry));
        }
    }
    return files;
}

std::list<std::filesystem::path> get_names(const std::filesystem::path &path) {
    std::list<std::filesystem::path> names;
    for (auto const &entry : std::filesystem::directory_iterator{path}) {
        std::filesystem::path filename = entry.path().filename();
        names.push_back(filename);
    }
    return names;
}

void unwind_files(const std::filesystem::path &path, const std::filesystem::path &parent) {
    auto files_to_unwind = get_files(path);
    auto current_names = get_names(parent);
    for (auto const &file : files_to_unwind) {
        int count = 1;
        std::string stem = file.filename().stem();
        std::string extension = file.filename().extension();
        std::string filename = file.filename();
        while (std::find(current_names.begin(), current_names.end(), filename) != current_names.end()) {
            filename = stem + "(" + std::to_string(count++) + ")" + extension;
        }
        current_names.push_back(filename);
        std::filesystem::path new_path = parent / filename;
        try {
            if (!std::filesystem::exists(new_path)) {
                std::filesystem::rename(file, new_path);
            } else {
                wxLogError(wxString(new_path.string() + " exists!"));
            }
        } catch (std::filesystem::filesystem_error &e) {
            wxLogError(wxString(filename + ": ") << e.what());
        }
    }
}

void CardPanel::OnFolderMenuClick(wxCommandEvent &evt) {
    switch (evt.GetId()) {
        case FOLDER_UNWIND:
            unwind_files(this->file.path, this->file.path.parent_path());
            break;
        case FOLDER_ORGANIZE:
            organize_file(this->file.path, this->parent->config);
            break;
    }
}

void CardPanel::OnFolderRightClick(wxMouseEvent &evt) {
    wxMenu menu;
    menu.Append(FOLDER_UNWIND, "Unwind files...");
    menu.Append(FOLDER_ORGANIZE, "Organize files...");
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CardPanel::OnFolderMenuClick), NULL, this);
    PopupMenu(&menu);
}

bool CheckPosition(wxRect rect, wxPoint pos, int box){
    return rect.GetX() < pos.x - box && rect.GetRight() > pos.x + box 
    && rect.GetY() < pos.y - box && rect.GetBottom() > pos.y + box;
}

void CardPanel::OnEnterPanel(wxMouseEvent& event) {
    this->image->ChangeLightness(130);
    this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHT));
    this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
}

void CardPanel::OnLeavePanel(wxMouseEvent& event) {
	auto mousePosition = ClientToScreen(event.GetPosition());
    auto rect = GetScreenRect();
    if(CheckPosition(rect, mousePosition, 0)){
        this->image->ChangeLightness(130);
        this->label->SetBackgroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHT));
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_HIGHLIGHTTEXT));
    }
    else{
        this->image->ChangeLightness(100);
        this->label->SetBackgroundColour(*wxWHITE);
        this->label->SetForegroundColour(wxSystemSettings::GetColour(wxSystemColour::wxSYS_COLOUR_BTNTEXT));
    }
}