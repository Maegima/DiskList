/**
 * @file DBUnsignedLong.cpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.3
 * @date 2024-03-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <sstream>
#include "DBUnsignedLong.hpp"

DBUnsignedLong::DBUnsignedLong(const char* name, bool not_null, bool index, bool key, bool unique, bool auto_increment) : DBType(name, not_null, index, key, unique), auto_increment(auto_increment) {
    type = "INTEGER";
}

inline unsigned long DBUnsignedLong::operator=(const unsigned long value) {
    value_set = true;
    return this->value = value;
}

inline void DBUnsignedLong::read(sqlite3_stmt *stmt) {
    *this = (unsigned long) sqlite3_column_int64(stmt, column);
}

std::ostringstream DBUnsignedLong::create() {
    std::ostringstream field;
    field << this->name << " " << this->type;
    if (this->key) field << " PRIMARY KEY";
    if (this->auto_increment) field << " AUTOINCREMENT";
    if (this->not_null) field << " NOT NULL";
    if (this->unique) field << " UNIQUE";
    return field;
}

inline std::string DBUnsignedLong::val() {
    std::stringstream ret;
    ret << value;
    return ret.str();
}