/**
 * @file ColumnViewInfo.cpp
 * @author your name (you@domain.com)
 * @brief ColumnViewInfo class implementation
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "ColumnViewInfo.hpp"

ColumnViewInfo::ColumnViewInfo(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
 : Gtk::ColumnView(cobject) {
    // Id column
    auto column = std::dynamic_pointer_cast<Gtk::ColumnViewColumn>(this->get_columns()->get_object(0));
    auto factory = (Gtk::SignalListItemFactory *) column->get_factory().get();
    factory->signal_setup().connect(
        sigc::bind(sigc::mem_fun(*this, &ColumnViewInfo::on_setup_label), Gtk::Align::END));
    factory->signal_bind().connect(
        sigc::mem_fun(*this, &ColumnViewInfo::on_bind_id));
    
    // Name column
    column = std::dynamic_pointer_cast<Gtk::ColumnViewColumn>(this->get_columns()->get_object(1));
    factory = (Gtk::SignalListItemFactory *) column->get_factory().get();
    factory->signal_setup().connect(sigc::bind(
        sigc::mem_fun(*this, &ColumnViewInfo::on_setup_label), Gtk::Align::START));
    factory->signal_bind().connect(
        sigc::mem_fun(*this, &ColumnViewInfo::on_bind_name));

    auto m_selection = (Gtk::SingleSelection *) this->get_model().get();
    auto m_ListStore = (Gio::ListStore<PropertyInfo> *)m_selection->get_model().get();
}

ColumnViewInfo::~ColumnViewInfo() noexcept {
}

void ColumnViewInfo::on_setup_label(const Glib::RefPtr<Gtk::ListItem>& list_item, Gtk::Align halign){
  list_item->set_child(*Gtk::make_managed<Gtk::Label>("", halign));
}

void ColumnViewInfo::on_bind_id(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
  auto col = std::dynamic_pointer_cast<PropertyInfo>(list_item->get_item());
  if (!col)
    return;
  auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
  if (!label)
    return;
  label->set_text(Glib::ustring::format(col->property));
}

void ColumnViewInfo::on_bind_name(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
  auto col = std::dynamic_pointer_cast<PropertyInfo>(list_item->get_item());
  if (!col)
    return;
  auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
  if (!label)
    return;
  label->set_text(col->value);
}


