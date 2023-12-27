/**
 * @file Image.hpp
 * @author André Lucas Maegima
 * @brief Image texture loader definition
 * @version 0.3
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_IMAGE_HPP
#define DISKLIST_IMAGE_HPP

#include <wx/wx.h>
#include <filesystem>
#include "Configuration.hpp"

class Image : public wxPanel {
   public:
    enum Type { STATIC, DYNAMIC };
    Image(wxWindow* parent, const std::filesystem::directory_entry& path, Configuration& config);
    ~Image();

    void OnPaint(wxPaintEvent& evt);
    void render(wxDC& dc);
    void OnSize(wxSizeEvent& event);
    void ChangeLightness(int alpha);

   private:
    wxImage* static_img;
    wxImage image;
    wxBitmap resized;
    int width;
    int height;
    Type type;
    bool changed;
};

#endif /* DISKLIST_IMAGE_HPP */