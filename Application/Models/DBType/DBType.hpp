/**
 * @file DBType.hpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _DBTYPE_HPP_
#define _DBTYPE_HPP_

#include <sqlite3.h>
#include <string>

class DBType {
   public:
    const char *name = nullptr;
    bool not_null = true;
    bool key = false;
    bool index = false;
    std::string type = "";
    int column = -1;
    bool value_set = false;

    DBType();
    DBType(const char *name, bool not_null = false, bool index = false, bool key = false);
    virtual void read(sqlite3_stmt *stmt) = 0;
    virtual std::string val() = 0;
    virtual std::ostringstream create();
};

#endif  // _DBTYPE_HPP_