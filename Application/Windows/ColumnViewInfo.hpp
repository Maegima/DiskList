/**
 * @file ColumnViewInfo.hpp
 * @author your name (you@domain.com)
 * @brief ColumnViewInfo class definition
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GTKMM_COLUMNVIEWINFO_HPP
#define GTKMM_COLUMNVIEWINFO_HPP

#include <gtkmm.h>

class ColumnViewInfo : public Gtk::ColumnView {
   public:
    ColumnViewInfo(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    ~ColumnViewInfo() noexcept override;

    void on_setup_label(const Glib::RefPtr<Gtk::ListItem>& list_item, Gtk::Align halign);

    void on_bind_id(const Glib::RefPtr<Gtk::ListItem>& list_item);

    void on_bind_name(const Glib::RefPtr<Gtk::ListItem>& list_item);

   protected:
    class PropertyInfo : public Glib::Object {
       public:
        std::string property;
        Glib::ustring value;

        static Glib::RefPtr<PropertyInfo> create(std::string property, const Glib::ustring& value) {
            return Glib::make_refptr_for_instance<PropertyInfo>(new PropertyInfo(property, value));
        }

       protected:
        PropertyInfo(std::string property, const Glib::ustring& value)
            : property(property), value(value) {}
    };
};

#endif /* GTKMM_COLUMNVIEWINFO_HPP */