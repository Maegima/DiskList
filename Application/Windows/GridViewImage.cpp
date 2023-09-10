/**
 * @file GridViewImage.cpp
 * @author André Lucas Maegima
 * @brief GridViewImage class implementation
 * @version 0.2
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include "GridViewImage.hpp"

GridViewImage::GridViewImage(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::GridView(cobject) {
    auto factory = (Gtk::SignalListItemFactory *)this->get_factory().get();
    if (!factory)
        throw std::runtime_error("No \"factory\" object in grid");

    m_selection_model = (Gtk::SingleSelection *)this->get_model().get();
    if (!m_selection_model)
        throw std::runtime_error("No \"model\" object in grid");

    m_data_model = (Gio::ListStore<ModelColumns> *)m_selection_model->get_model().get();
    if (!m_data_model)
        throw std::runtime_error("No \"data model\" object in grid");
    std::cout << "Item activated: filename=" << std::endl;

    factory->signal_setup().connect(
        sigc::mem_fun(*this, &GridViewImage::on_listitem_setup));
    factory->signal_bind().connect(
        sigc::mem_fun(*this, &GridViewImage::on_listitem_bind));

    m_selection_model->property_selected().signal_changed().connect(
        sigc::mem_fun(*this, &GridViewImage::on_selection_changed));

    m_data_model->sort(
        sigc::mem_fun(*this, &GridViewImage::on_model_sort));

    this->signal_activate().connect(sigc::mem_fun(*this, &GridViewImage::on_activate));
}

void GridViewImage::on_activate(guint position) {
    auto col = m_data_model->get_item(position);
    if (!col) return;

    const std::string filename = col->m_filename;
    const Glib::ustring description = col->m_description;

    std::cout << "Item activated: filename=" << filename << ", description=" << description
              << std::endl;
}

void GridViewImage::on_selection_changed() {
    auto position = m_selection_model->get_selected();
    auto col = m_data_model->get_item(position);
    if (!col) return;

    const std::string filename = col->m_filename;
    const Glib::ustring description = col->m_description;

    std::cout << "Selection Changed to: filename=" << filename << ", description=" << description
              << std::endl;
}

int GridViewImage::on_model_sort(const Glib::RefPtr<const ModelColumns> &a,
                                 const Glib::RefPtr<const ModelColumns> &b) {
    return (a->m_description < b->m_description) ? -1
                                                 : ((a->m_description > b->m_description) ? 1 : 0);
}

void GridViewImage::on_listitem_setup(const Glib::RefPtr<Gtk::ListItem>& list_item){
    // Each ListItem contains a vertical Box with an Image and a Label.
    auto vBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    auto picture = Gtk::make_managed<Gtk::Picture>();
    picture->set_can_shrink(false);
    picture->set_halign(Gtk::Align::CENTER);
    picture->set_valign(Gtk::Align::END);
    vBox->append(*picture);
    auto label = Gtk::make_managed<Gtk::Label>();
    label->set_wrap(true);
    label->set_wrap_mode(Pango::WrapMode::CHAR);
    label->set_max_width_chars(15);
    label->set_width_chars(12);
    vBox->append(*label);
    list_item->set_child(*vBox);
}

void GridViewImage::on_listitem_bind(const Glib::RefPtr<Gtk::ListItem>& list_item){
    auto col = std::dynamic_pointer_cast<ModelColumns>(list_item->get_item());
    if (!col) return;
    auto vBox = dynamic_cast<Gtk::Box *>(list_item->get_child());
    if (!vBox) return;
    auto picture = dynamic_cast<Gtk::Picture *>(vBox->get_first_child());
    if (!picture) return;
    auto label = dynamic_cast<Gtk::Label *>(picture->get_next_sibling());
    if (!label) return;
    picture->set_paintable(col->m_texture);
    label->set_markup(col->m_description);
}

void GridViewImage::add_entry(const std::string &filename, const Glib::ustring &description) {
    try {
        auto pixbuf = Gdk::Pixbuf::create_from_file(filename, 50, 200);
        auto texture = Gdk::Texture::create_for_pixbuf(pixbuf);
        m_data_model->append(ModelColumns::create(filename, description, texture));
    } catch (const Gdk::PixbufError &ex) {
        std::cerr << "Gdk::PixbufError: " << ex.what() << std::endl;
    } catch (const Glib::FileError &ex) {
        std::cerr << "Glib::FileError: " << ex.what() << std::endl;
    }
}