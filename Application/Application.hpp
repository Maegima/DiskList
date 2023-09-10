/**
 * @file Application.hpp
 * @author André Lucas Maegima
 * @brief GTKMM Application override definitions
 * @version 0.2
 * @date 2023-09-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GTKMM_APPLICATION_HPP
#define GTKMM_APPLICATION_HPP

#include <gtkmm.h>

class MainWindow;

class Application : public Gtk::Application {
   protected:
    Application();

   public:
    static Glib::RefPtr<Application> create();

   protected:
    // Override default signal handlers:
    void on_activate() override;
    void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;

   private:
    MainWindow* create_appwindow();
};

#endif /* GTKMM_APPLICATION_HPP */
