/**
 * @file CardPanel.cpp
 * @author André Lucas Maegima
 * @brief CardPanel class implementation
 * @version 0.3
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "CardPanel.hpp"
#include "ListingWindow.hpp"

std::map<std::string, wxImage*> CardPanel::default_images;

void CardPanel::InitializeDefaultIcons(std::map<std::string, std::string> config) {
    for(auto &[key, value] : config){
        default_images.insert({key, new wxImage(value, wxBITMAP_TYPE_PNG)});
    }
}

CardPanel::CardPanel(ListingWindow* parent, std::filesystem::directory_entry entry, wxString path) 
    : wxPanel(parent, wxID_ANY), 
    parent(parent),
    file(FileInfo(entry, false)),
    image(CreateImage(entry)), 
    label(CreateLabel(entry, path))
    {
    auto sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
    sizer->AddSpacer(5);
    sizer->Add(image, 0);
    sizer->Add(label, 0, wxALIGN_CENTER);
    sizer->AddSpacer(10);
}

CardPanel::~CardPanel() {
    delete image;
    delete label;
}

wxStaticText* CardPanel::CreateLabel(std::filesystem::directory_entry entry, wxString path) {
    wxString name = path.IsEmpty() ? wxString::FromUTF8(entry.path().filename()) : path;
    wxStaticText *text = new wxStaticText(this, wxID_ANY, name, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    int max_text_size = 180;
    while(text->m_width > 200) {
        int parts = text->m_width/max_text_size;
        int size = name.length()/(parts + 1);
        wxString newstr = "";
        for(int i = 0; i < parts; i++){
            newstr += name.substr(i*size, size) + "\n";
        }
        newstr += name.substr(parts*size);
        text->SetLabel(newstr);
        max_text_size -= 10;
    }
    text->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnTextClick, this, wxID_ANY);
    return text;
}

Image* CardPanel::CreateImage(std::filesystem::directory_entry entry) {
    Image *img = nullptr;
    std::string extension = entry.path().extension().string();
    if (entry.is_directory()) {
        img = new Image(this, default_images["folder"]);   
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFolderLeftClick, this, wxID_ANY);
        img->Bind(wxEVT_RIGHT_DOWN, &CardPanel::OnFolderRightClick, this, wxID_ANY);
    } else if (extension == ".png") {
        wxString path = wxString::FromUTF8(entry.path());
        img = new Image(this, new wxImage(path, wxBITMAP_TYPE_PNG), Image::Type::DYNAMIC);
    } 
    else if(default_images.contains(extension)){
        img = new Image(this, default_images[extension]);
    } else {
        img = new Image(this, default_images["default"]);
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFileClick, this, wxID_ANY);
    }
    return img;
    
}

void CardPanel::OnTextClick(wxMouseEvent& event) {
    std::cout << label->GetLabel() << "\n";
    event.Skip();
}

void CardPanel::OnFolderLeftClick(wxMouseEvent& event) {
    std::cout << label->GetLabel() << " " << parent->current << "\n";
    parent->ChangePath(file.path);
}

void CardPanel::OnFileClick(wxMouseEvent& event) {
    std::cout << label->GetLabel() << " " << parent->current << " file\n";
    std::list<std::pair<wxString, wxString>> list;
    list.push_back({"Name", file.path.filename().string()});
    list.push_back({"Size", wxString() << file.size_str()});
    list.push_back({"Created", wxString() << file.created_str()});
    list.push_back({"Modified", wxString() << file.modified_str()});
    list.push_back({"Accessed", wxString() << file.accessed_str()});
    parent->iwindow->FillGrid(list);
}

bool CardPanel::CompareCards::operator() (const CardPanel* c1, const CardPanel* c2) const {
    if(c1->file.type != c2->file.type)
        return c1->file.type == FileType::Directory;
    return c1->file.path < c2->file.path;
}

void CardPanel::OnFolderMenuClick(wxCommandEvent &evt) {
    switch (evt.GetId()) {
        case FOLDER_UNWIND:     
            break;
        case FOLDER_ORGANIZE:
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