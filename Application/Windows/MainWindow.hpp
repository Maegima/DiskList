/**
 * @file MainWindow.hpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Definitions
 * @version 0.3
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023
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
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    ListingWindow* lwindow;
    InfoWindow* iwindow;
};

#endif /* DISKLIST_MAINWINDOW_HPP_ */
