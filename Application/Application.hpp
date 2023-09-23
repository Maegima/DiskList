/**
 * @file Application.hpp
 * @author André Lucas Maegima
 * @brief Disklist Application definitions
 * @version 0.2
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DISKLIST_APPLICATION_HPP
#define DISKLIST_APPLICATION_HPP

#include "Windows/MainWindow.hpp"

class Application {
   protected:
    MainWindow *window;

   public:
    Application();
    ~Application();
    void Run(MainWindow *window);
};

#endif /* DISKLIST_APPLICATION_HPP */
