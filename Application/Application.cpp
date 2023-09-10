/**
 * @file Application.cpp
 * @author André Lucas Maegima
 * @brief GTKMM Application override implementations
 * @version 0.2
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <list>
#include <iostream>
#include <exception>
#include "Application.hpp"
#include "Windows/MainWindow.hpp"

Application::Application() : Gtk::Application("disklist.application", Gio::Application::Flags::HANDLES_OPEN) {}

Glib::RefPtr<Application> Application::create() {
    return Glib::make_refptr_for_instance<Application>(new Application());
}

MainWindow* Application::create_appwindow() {
    auto appwindow = MainWindow::create();

    // Make sure that the application runs for as long this window is still
    // open.
    add_window(*appwindow);

    // A window can be added to an application with
    // Gtk::Application::add_window() or Gtk::Window::set_application(). When
    // all added windows have been hidden or removed, the application stops
    // running (Gtk::Application::run() returns()), unless
    // Gio::Application::hold() has been called.

    // Delete the window when it is hidden.
    appwindow->signal_hide().connect([appwindow]() { delete appwindow; });

    return appwindow;
}

void Application::on_activate() {
    try {
        // The application has been started, so let's show a window.
        auto appwindow = create_appwindow();
        appwindow->present();
    }
    // If create_appwindow() throws an exception (perhaps from Gtk::Builder),
    // no window has been created, no window has been added to the application,
    // and therefore the application will stop running.
    catch (const Glib::Error& ex) {
        std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Application::on_activate(): " << ex.what() << std::endl;
    }
}

void Application::on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& /* hint */) {
    // The application has been asked to open some files,
    // so let's open a new view for each one.
    MainWindow* appwindow = nullptr;
    auto windows = get_windows();
    if (windows.size() > 0) appwindow = dynamic_cast<MainWindow*>(windows[0]);

    try {
        if (!appwindow) appwindow = create_appwindow();

        for (const auto& file : files) appwindow->open_file_view(file);

        appwindow->present();
    } catch (const Glib::Error& ex) {
        std::cerr << "Application::on_open(): " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Application::on_open(): " << ex.what() << std::endl;
    }
}
