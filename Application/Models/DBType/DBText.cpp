/**
 * @file DBText.cpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <string.h>
#include <sstream>
#include "DBText.hpp"

DBText::DBText(const char *name, bool not_null, bool index, bool key)
    : DBType(name, not_null, index, key) {
    type = "TEXT";
}

DBText::~DBText() {
    if (value != nullptr)
        free(value);
}

inline unsigned char *DBText::operator=(const unsigned char *value) {
    value_set = true;
    if (this->value != nullptr)
        free(this->value);
    int len = strlen((const char *)value) + 1;
    this->value = (unsigned char *)malloc(sizeof(unsigned char *) * len);
    memcpy(this->value, value, len);
    return this->value;
}

inline void DBText::read(sqlite3_stmt *stmt) {
    *this = sqlite3_column_text(stmt, column);
}

inline std::string DBText::val() {
    std::stringstream ret;
    ret << value;
    return ret.str();
}
