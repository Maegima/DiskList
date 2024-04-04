/**
 * @file MainWindow.hpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Definitions
 * @version 0.4
 * @date 2024-04-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DISKLIST_MAINWINDOW_HPP
#define DISKLIST_MAINWINDOW_HPP

#include "ListingWindow.hpp"
#include "InfoWindow.hpp"

enum {
    ID_Hello = 1
};

class MainWindow : public wxFrame {
   public:
    MainWindow();

   private:
    void OnBackward(wxEvent& event);
    void OnForward(wxEvent& event);
    void OnKeyPress(wxKeyEvent& event);

    ListingWindow* lwindow;
    InfoWindow* iwindow;
    wxBitmapButton *forward;
    wxBitmapButton *backward;
};

#endif /* DISKLIST_MAINWINDOW_HPP_ */
