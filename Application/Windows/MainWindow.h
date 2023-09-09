/**
 * @file MainWindow.h
 * @author André Lucas Maegima
 * @brief Disklist Main Window Definitions
 * @version 0.2
 * @date 2023-09-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GTKMM_MAINWINDOW_H_
#define GTKMM_MAINWINDOW_H_

#include <gtkmm.h>

class MainWindow : public Gtk::ApplicationWindow {
   public:
    MainWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);

    void open_file_view(const Glib::RefPtr<Gio::File> &file);
    void add_entry(const std::string &filename, const Glib::ustring &description);

   protected:
    class ModelColumns;
    Glib::RefPtr<Gtk::Builder> m_refBuilder;

    void on_item_activated(unsigned int position);
    void on_selection_changed();
    void on_setup_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_bind_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item);
    int on_model_sort(const Glib::RefPtr<const ModelColumns> &a, const Glib::RefPtr<const ModelColumns> &b);


    // A Gio::ListStore stores filename, description and texture.
    class ModelColumns : public Glib::Object {
       public:
        std::string m_filename;
        Glib::ustring m_description;
        Glib::RefPtr<Gdk::Texture> m_texture;

        static Glib::RefPtr<ModelColumns> create(const std::string &filename, const Glib::ustring &description,
                                                 const Glib::RefPtr<Gdk::Texture> &texture) {
            return Glib::make_refptr_for_instance<ModelColumns>(new ModelColumns(filename, description, texture));
        }

       protected:
        ModelColumns(const std::string &filename, const Glib::ustring &description, const Glib::RefPtr<Gdk::Texture> &texture)
            : m_filename(filename), m_description(description), m_texture(texture) {}
    };  // ModelColumns

    Gio::ListStore<ModelColumns> *m_data_model;
    Gtk::SingleSelection *m_selection_model;
};

#endif /* GTKMM_MAINWINDOW_H_ */
