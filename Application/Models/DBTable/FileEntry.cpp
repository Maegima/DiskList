/**
 * @file FileEntry.cpp
 * @author André Lucas Maegima
 * @brief 
 * @version 0.2
 * @date 2023-10-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "FileEntry.hpp"

FileEntry::FileEntry(sqlite3 *db) : DBTable(db) {
    this->table_name = "FileEntry";

    this->types = DBTypeDict({&id, &path, &hash, &size, &type, &created, &modified, &accessed});
}
 