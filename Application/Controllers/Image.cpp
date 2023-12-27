/**
 * @file Image.cpp
 * @author André Lucas Maegima
 * @brief Image texture loader implementation
 * @version 0.3
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Image.hpp"

Image::Image(wxWindow* parent, const std::filesystem::directory_entry &entry, Configuration &config) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200, 200)) {
    std::vector<std::string> img_exts = config.image_extension;
    std::string extension = entry.path().extension().string();
    std::string key = "default";
    if (entry.is_directory()) {
        key = "folder";
    } else if (std::find(img_exts.begin(), img_exts.end(), extension) != img_exts.end()) {
        key = "dynamic";
    } else if (config.image.contains(extension)) {
        key = extension;
    }
    if(key == "dynamic") {
        this->type = Type::DYNAMIC; 
        this->static_img = new wxImage(wxString::FromUTF8(entry.path()), wxBITMAP_TYPE_ANY);
    } else {
        this->type = Type::STATIC;
        this->static_img = config.image[key];
    }
    this->image = *static_img;
    width = -1;
    height = -1;
    changed = false;
    Bind(wxEVT_PAINT, &Image::OnPaint, this, wxID_ANY);
}

Image::~Image() {
    if(type == Image::Type::DYNAMIC)
        delete static_img;
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
