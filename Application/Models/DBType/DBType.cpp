/**
 * @file DBType.cpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <sstream>
#include "DBType.hpp"

DBType::DBType() {}

DBType::DBType(const char *name, bool not_null, bool index, bool key, bool unique)
    : name(name), not_null(not_null), index(index), key(key), unique(unique) {}

std::ostringstream DBType::create() {
    std::ostringstream field;
    field << this->name << " " << this->type;
    if (this->key) field << " PRIMARY KEY";
    if (this->not_null) field << " NOT NULL";
    if (this->unique) field << " UNIQUE";
    return field;
}