/**
 * @file Image.cpp
 * @author André Lucas Maegima
 * @brief Image texture loader implementation
 * @version 0.3
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Image.hpp"

Image::Image(wxWindow* parent, wxImage *image, Type type) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200, 200)) {
    this->image = image;
    this->type = type;
    width = -1;
    height = -1;
    Bind(wxEVT_PAINT, &Image::OnPaint, this, wxID_ANY);
}

Image::~Image() {
    if(type == Image::Type::DYNAMIC)
        delete image;
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

    if (neww != width || newh != height) {
        width = image->GetWidth() * ((double)newh / (double)image->GetHeight());
        height = newh;
        if (width > neww) {
            height = image->GetHeight() * ((double)neww / (double)image->GetWidth());
            width = neww;
        }
        resized = wxBitmap(image->Scale(width, height /*, wxIMAGE_QUALITY_HIGH*/));
        dc.DrawBitmap(resized, (neww - width) / 2, (newh - height) / 2, false);
    } else {
        dc.DrawBitmap(resized, (neww - width) / 2, (newh - height) / 2, false);
    }
}
