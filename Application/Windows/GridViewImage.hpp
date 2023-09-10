/**
 * @file GridViewImage.hpp
 * @author André Lucas Maegima
 * @brief GridViewImage class definition
 * @version 0.2
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GTKMM_GRIDVIEWIMAGE_HPP_
#define GTKMM_GRIDVIEWIMAGE_HPP_

#include <gtkmm.h>

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

class GridViewImage : public Gtk::GridView {
   public:
    GridViewImage(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder);
    void add_entry(const std::string &filename, const Glib::ustring &description);

   protected:
    Gio::ListStore<ModelColumns> *m_data_model;
    Gtk::SingleSelection *m_selection_model;

    void on_activate(guint position);
    void on_selection_changed();
    void on_listitem_setup(const Glib::RefPtr<Gtk::ListItem> &list_item);
    void on_listitem_bind(const Glib::RefPtr<Gtk::ListItem> &list_item);
    int on_model_sort(const Glib::RefPtr<const ModelColumns> &a,
                      const Glib::RefPtr<const ModelColumns> &b);
};

#endif /* GTKMM_GRIDVIEWIMAGE_HPP_ */