/**
 * @file CardPanel.hpp
 * @author André Lucas Maegima
 * @brief CardPanel class definition
 * @version 0.3
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _CARDPANEL_HPP_
#define _CARDPANEL_HPP_

#include <wx/wx.h>
#include <filesystem>
#include <map>
#include "Controllers/Image.hpp"

class CardPanel : public wxPanel {
    public:
        Image *image;
        wxStaticText *label;
        
        CardPanel(wxWindow* parent, std::filesystem::directory_entry entry);
        
        void OnFolderClick(wxMouseEvent& event);
        void OnTextClick(wxMouseEvent& event);
        static void InitializeDefaultIcons(const char* path);
    private:
        static std::map<std::string, wxImage*> default_images;
        wxStaticText *CreateLabel(std::filesystem::directory_entry entry);
        Image *CreateImage(std::filesystem::directory_entry entry);
};

#endif // _CARDPANEL_HPP_