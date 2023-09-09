/**
 * @file MainWindow.h
 * @author André Lucas Maegima
 * @brief Disklist Main Window Definitions
 * @version 0.2
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GTKMM_MAINWINDOW_H_
#define GTKMM_MAINWINDOW_H_

#include <gtkmm.h>
#include "GridViewImage.h"

class MainWindow : public Gtk::ApplicationWindow {
   public:
    MainWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);

    void open_file_view(const Glib::RefPtr<Gio::File> &file);
    static MainWindow* create();

   protected:
    GridViewImage* grid_view;
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
};

#endif /* GTKMM_MAINWINDOW_H_ */
