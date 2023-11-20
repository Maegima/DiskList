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
    if(path.has_parent_path()){
        std::filesystem::directory_entry parent_entry(path.parent_path());
        cards.insert(new CardPanel(this, parent_entry, ".."));
    }
    for (auto const& entry : std::filesystem::directory_iterator{current}) {        
        cards.insert(new CardPanel(this, entry));
    }
    for (auto const& card : cards) {
        sizer->Add(card, 0, wxLEFT | wxRIGHT, 0);
    }
    this->SetScrollbars(0, 40, 0, sizer->GetSize().GetHeight()/40);
    this->SendSizeEvent();
    this->Refresh();
}
