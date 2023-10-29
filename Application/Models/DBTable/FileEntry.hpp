
/**
 * @file FileEntry.hpp
 * @author André Lucas Maegima
 * @brief 
 * @version 0.2
 * @date 2023-10-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _FILEENTRY_HPP_
#define _FILEENTRY_HPP_

#include "DBTable.hpp"
#include "Models/DBType/DBInt.hpp"
#include "Models/DBType/DBText.hpp"

class FileEntry : public DBTable {
   public:
    DBInt id = DBInt("id", true, true, true, false, true);
    DBText path = DBText("path", true, true, false, true);
    DBText hash = DBText("hash", true, true);
    DBInt size = DBInt("size");
    DBInt type = DBInt("type");
    DBText created = DBText("created");
    DBText modified = DBText("modified");
    DBText accessed = DBText("accessed");

    FileEntry(sqlite3 *db);
};

#endif // _FILEENTRY_HPP_