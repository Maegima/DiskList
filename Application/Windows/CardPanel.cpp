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

#include <fstream>
#include "CardPanel.hpp"
#include "ListingWindow.hpp"

std::map<std::string, wxImage*> CardPanel::default_images;

void CardPanel::InitializeDefaultIcons(const char* path) {
    std::fstream file(path, std::ios::in);
    std::string data;
    while (getline(file, data)) {
        size_t pos = data.find('=');
        if (pos != std::string::npos) {
            wxImage* img = new wxImage(data.substr(pos + 1).c_str(), wxBITMAP_TYPE_PNG);
            std::string ext = data.substr(0, pos);
            default_images.insert({ext, img});
            std::cout << data << " " << img->GetHeight() << " " << img->GetWidth() << std::endl;
        }
    }
}

CardPanel::CardPanel(ListingWindow* parent, std::filesystem::directory_entry entry, wxString path) 
    : wxPanel(parent, wxID_ANY), 
    parent(parent),
    entry(entry),
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
        img->Bind(wxEVT_LEFT_DOWN, &CardPanel::OnFolderClick, this, wxID_ANY);
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

void CardPanel::OnFolderClick(wxMouseEvent& event) {
    std::cout << label->GetLabel() << " " << parent->current << "\n";
    parent->ChangePath(entry);
}

void CardPanel::OnFileClick(wxMouseEvent& event) {
    std::cout << label->GetLabel() << " " << parent->current << " file\n";
    std::list<std::pair<wxString, wxString>> list;
    list.push_back({"Name", entry.path().filename().string()});
    list.push_back({"Size", wxString() << entry.file_size()});
    parent->iwindow->FillGrid(list);
}

bool CardPanel::CompareCards::operator() (const CardPanel* c1, const CardPanel* c2) const {
    if(c1->entry.is_directory() && !c2->entry.is_directory())
        return true;
    else if(!c1->entry.is_directory() && c2->entry.is_directory())
        return false;
    return c1->entry.path() < c2->entry.path();
}