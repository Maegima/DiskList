/**
 * @file Image.hpp
 * @author André Lucas Maegima
 * @brief Image texture loader definition
 * @version 0.3
 * @date 2024-04-01
 *
 * @copyright Copyright (c) 2024
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
    std::string path;
    int width;
    int height;
    bool changed;
    Type type;
    wxImage* static_img;
    wxImage* default_img;
    wxImage image;
    wxBitmap resized;

    void LoadImage();
    std::string GetKey(const std::filesystem::directory_entry& path, Configuration& config);
};

#endif /* DISKLIST_IMAGE_HPP */