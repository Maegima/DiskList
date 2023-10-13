/**
 * @file DBTable.hpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _DBTABLE_HPP_
#define _DBTABLE_HPP_

#include <sqlite3.h>
#include <string>
#include "DBTypeDict.hpp"

class DBTable {
   protected:
    std::string table_name = "";
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;  // will point to prepared stamement object

    DBTypeDict types;

   public:
    std::string error;
    DBTable(sqlite3 *db);

    virtual ~DBTable();

    int execute(const char *sql);

    bool save();

    bool create();

    std::string indexes();

    bool prepare();

    bool step();

    bool finalize();
};

#endif  // _DBTABLE_HPP_