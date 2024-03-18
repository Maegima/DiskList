
/**
 * @file DBUnsignedLong.hpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.3
 * @date 2024-03-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef _DBUNSIGNEDLONG_HPP_
#define _DBUNSIGNEDLONG_HPP_

#include "DBType.hpp"

class DBUnsignedLong : public DBType {
   public:
    unsigned long value = 0;
    bool auto_increment = false;

    DBUnsignedLong(const char *name, bool not_null = true, bool index = false, bool key = false, bool unique = false, bool auto_increment = false);
    unsigned long operator=(const unsigned long value);
    void read(sqlite3_stmt *stmt);
    std::string val();
    std::ostringstream create() override;
};

#endif // _DBUNSIGNEDLONG_HPP_