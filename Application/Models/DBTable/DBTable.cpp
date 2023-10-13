/**
 * @file DBTable.cpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <sstream>
#include "DBTable.hpp"

DBTable::DBTable(sqlite3 *db) : db(db){};

DBTable::~DBTable() {
    if (stmt != nullptr)
        finalize();
}

int DBTable::execute(const char *sql) {
    char *zErrMsg;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &zErrMsg);
    if (rc != SQLITE_OK) {
        error = zErrMsg;
        sqlite3_free(zErrMsg);
    }
    return rc;
}

bool DBTable::save() {
    std::ostringstream sql;
    sql << "INSERT INTO " << this->table_name << "(";
    for (const auto &[key, value] : this->types) {
        if (value->value_set) {
            sql << "`" << value->name << "`,";
        }
    }
    sql.seekp(-1, std::ios_base::end);
    sql << ") VALUES (";
    for (const auto &[key, value] : this->types) {
        if (value->value_set)
            sql << "\"" << value->val() << "\",";
    }
    sql.seekp(-1, std::ios_base::end);
    sql << ");";
    fprintf(stderr, "%s\n", sql.str().c_str());
    return execute(sql.str().c_str()) == SQLITE_OK;
}

bool DBTable::create() {
    std::ostringstream sql;
    sql << "CREATE TABLE " << table_name << "(";
    for (auto &[key, value] : this->types) {
        sql << value->create().str() << ",";
    }
    sql.seekp(-1, std::ios_base::end);
    sql << ");";
    sql << indexes();

    return execute(sql.str().c_str()) == SQLITE_OK;
}

std::string DBTable::indexes() {
    std::ostringstream sql;
    for (const auto &[key, value] : this->types) {
        if (value->index)
            sql << "CREATE INDEX " << this->table_name << "_" << value->name << "_idx ON "
                << this->table_name << "(" << value->name << ");";
    }
    return sql.str();
}

bool DBTable::prepare() {
    std::string sql = "SELECT * from " + table_name;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK)
        return false;

    int col = sqlite3_column_count(stmt);
    for (int i = 0; i < col; i++) {
        const char *name = sqlite3_column_name(stmt, i);
        if (this->types.contains(name)) {
            this->types[name]->column = i;
        }
    }
    return true;
}

bool DBTable::step() {
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
        return false;
    for (auto &[key, value] : this->types) {
        if (value->column > -1) {
            value->read(stmt);
        }
    }
    return true;
}

bool DBTable::finalize() {
    int rc = sqlite3_finalize(stmt);
    stmt = nullptr;
    return rc == SQLITE_OK;
}