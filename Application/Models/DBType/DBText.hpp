/**
 * @file DBText.hpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _DBTEXT_HPP_
#define _DBTEXT_HPP_

#include "DBType.hpp"

class DBText : public DBType {
   public:
    unsigned char *value = nullptr;

    DBText(const char *name, bool not_null = true, bool index = false, bool key = false);
    ~DBText();

    unsigned char *operator=(const unsigned char *value);
    void read(sqlite3_stmt *stmt);
    std::string val();
};

#endif  // _DBTEXT_HPP_