/**
 * @file DBInt.cpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <sstream>
#include "DBInt.hpp"

DBInt::DBInt(const char *name, bool not_null, bool index, bool key, bool auto_increment)
    : DBType(name, not_null, index, key), auto_increment(auto_increment) {
    type = "INTEGER";
}

inline int DBInt::operator=(const int value) {
    value_set = true;
    return this->value = value;
}

inline void DBInt::read(sqlite3_stmt *stmt) {
    *this = sqlite3_column_int(stmt, column);
}

std::ostringstream DBInt::create() {
    std::ostringstream field;
    field << this->name << " " << this->type;
    if (this->key) field << " PRIMARY KEY";
    if (this->auto_increment) field << " AUTOINCREMENT";
    if (this->not_null) field << " NOT NULL";
    return field;
}

inline std::string DBInt::val() {
    std::stringstream ret;
    ret << value;
    return ret.str();
}
