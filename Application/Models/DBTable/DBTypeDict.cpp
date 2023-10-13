/**
 * @file DBTypeDict.cpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "DBTypeDict.hpp"

DBTypeDict::DBTypeDict() {}

DBTypeDict::DBTypeDict(std::list<DBType *> list) {
    for (DBType *item : list) {
        types[item->name] = item;
    }
}
