/**
 * @file Application.hpp
 * @author André Lucas Maegima
 * @brief Disklist Application definitions
 * @version 0.3
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_APPLICATION_HPP
#define DISKLIST_APPLICATION_HPP

#include <wx/wx.h>

class Application : public wxApp {
   public:
    virtual bool OnInit();    
};

#endif /* DISKLIST_APPLICATION_HPP */
