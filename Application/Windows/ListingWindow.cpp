/**
 * @file ListingWindow.cpp
 * @author André Lucas Maegima
 * @brief Listing window implementation
 * @version 0.3
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "wx/wrapsizer.h"
#include "ListingWindow.hpp"

ListingWindow::ListingWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxScrolledWindow(parent, id, pos, size, wxSUNKEN_BORDER) {
    SetBackgroundColour(*wxWHITE);
    CardPanel::InitializeDefaultIcons(".conf");
    
    Bind(wxEVT_SIZE, &ListingWindow::OnSize, this, wxID_ANY);
    
    wxWrapSizer *sizer = new wxWrapSizer(wxHORIZONTAL);
    SetSizer(sizer);
    ChangePath(std::filesystem::current_path());

    this->SetScrollbars(0, 40, 0, sizer->GetSize().GetHeight()/40);
    
}

void ListingWindow::OnSize(wxSizeEvent& event) {
    int width = this->m_width;
    int items = width/200;
    int spacing = items ? (width%200)/items : 0;
    auto sizer = GetSizer();
    for(auto &item : sizer->GetChildren()){
        item->SetBorder(spacing/2);
    }
    //auto newSize = this->m_parent->GetClientSize();
    //this->SetSize(newSize.x, newSize.y - 40);
    Refresh();
    event.Skip();
}

void ListingWindow::ChangePath(std::filesystem::path path) {
    auto *sizer = this->GetSizer();
    sizer->Clear(true);
    this->cards.clear();
    this->current = path;
    for (auto const& entry : std::filesystem::directory_iterator{current}) {        
        CardPanel *card = new CardPanel(this, entry);
        sizer->Add(card, 0, wxLEFT | wxRIGHT, 0);
        cards.push_back(card);
    }
    this->SendSizeEvent();
    this->Refresh();
    this->SetScrollbars(0, 40, 0, sizer->GetSize().GetHeight()/40);
}
