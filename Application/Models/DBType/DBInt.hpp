/**
 * @file DBInt.hpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _DBINT_HPP_
#define _DBINT_HPP_

#include "DBType.hpp"

class DBInt : public DBType {
   public:
    int value = 0;
    bool auto_increment = false;

    DBInt(const char *name, bool not_null = true, bool index = false, bool key = false, bool auto_increment = false);
    int operator=(const int value);
    void read(sqlite3_stmt *stmt);
    std::string val();
    std::ostringstream create() override;
};

#endif  // _DBINT_HPP_