/**
 * @file MainWindow.hpp
 * @author André Lucas Maegima
 * @brief Disklist Main Window Definitions
 * @version 0.2
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GTKMM_MAINWINDOW_HPP_
#define GTKMM_MAINWINDOW_HPP_

#include <gtkmm.h>
#include "GridViewImage.hpp"

class MainWindow : public Gtk::ApplicationWindow {
   public:
    MainWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);

    void open_file_view(const Glib::RefPtr<Gio::File> &file);
    static MainWindow* create();

   protected:
    GridViewImage* grid_view;
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
};

#endif /* GTKMM_MAINWINDOW_HPP_ */
