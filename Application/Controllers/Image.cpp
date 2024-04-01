/**
 * @file Image.cpp
 * @author André Lucas Maegima
 * @brief Image texture loader implementation
 * @version 0.3
 * @date 2024-04-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "Image.hpp"
#include "Algorithm.hpp"

Image::Image(wxWindow* parent, const std::filesystem::directory_entry& entry, Configuration &config)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200, 200)),
    path(entry.path()),
    width(-1),
    height(-1),
    changed(false) {
    std::string key = GetKey(entry, config);
    if(key == "dynamic") {
        this->type = Type::DYNAMIC;
        this->default_img = config.image["default"];
        this->static_img = nullptr;
    } else {
        this->type = Type::STATIC;
        this->default_img = config.image[key];
        this->static_img = default_img;
    }
    this->image = *default_img;
    Bind(wxEVT_PAINT, &Image::OnPaint, this, wxID_ANY);
}

Image::~Image() {
    if (type == Image::Type::DYNAMIC)
        delete static_img;
}

void Image::LoadImage() { 
    if (this->static_img == nullptr && this->type == Type::DYNAMIC) {
        this->static_img = new wxImage();
        this->static_img->SetLoadFlags(0);
        if (!this->static_img->LoadFile(wxString::FromUTF8(path), wxBITMAP_TYPE_ANY)) {    
            delete this->static_img;
            this->static_img = default_img;
            this->type = Type::STATIC;
        }
        this->image = *static_img;
    }
}

std::string Image::GetKey(const std::filesystem::directory_entry &entry, Configuration& config) {
    std::vector<std::string> img_exts = config.image_extension;
    std::string extension = entry.path().extension().string();
    if (entry.is_directory()) {
        return "folder";
    } else if (Algorithm::contains(img_exts, extension)) {
        return "dynamic";
    } else if (config.image.contains(extension)) {
        return extension;
    }
    return "default";
}

void Image::ChangeLightness(int alpha) {
    image = static_img->ChangeLightness(alpha);
    changed = true;
    Refresh();
}

void Image::OnPaint(wxPaintEvent& evt) {
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Here we call refresh to tell the panel to draw itself again.
 * So when the user resizes the image panel the image should be resized too.
 */
void Image::OnSize(wxSizeEvent& event) {
    // skip the event.
    Refresh();
    event.Skip();
}

void Image::render(wxDC& dc) {
    int neww, newh;
    LoadImage();
    dc.GetSize(&neww, &newh);

    if (neww != width || newh != height || changed) {
        width = image.GetWidth() * ((double)newh / (double)image.GetHeight());
        height = newh;
        if (width > neww) {
            height = image.GetHeight() * ((double)neww / (double)image.GetWidth());
            width = neww;
        }
        resized = wxBitmap(image.Scale(width, height /*, wxIMAGE_QUALITY_HIGH*/));
        dc.DrawBitmap(resized, (neww - width) / 2, (newh - height) / 2, false);
        changed = false;
    } else {
        dc.DrawBitmap(resized, (neww - width) / 2, (newh - height) / 2, false);
    }
}
