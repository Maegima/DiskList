/**
 * @file Image.hpp
 * @author André Lucas Maegima
 * @brief Image texture loader definition
 * @version 0.3
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_IMAGE_HPP
#define DISKLIST_IMAGE_HPP

#include <wx/wx.h>

class Image : public wxPanel {
   public:
    enum Type{ STATIC, DYNAMIC };
    Image(wxWindow* parent, wxImage *image, Image::Type type = Image::Type::STATIC);
    ~Image();

    void OnPaint(wxPaintEvent& evt);
    void render(wxDC& dc);
    void OnSize(wxSizeEvent& event);
   
   private:
    wxImage *image;
    wxBitmap resized;
    int width; 
    int height;
    Type type;
};

#endif /* DISKLIST_IMAGE_HPP */