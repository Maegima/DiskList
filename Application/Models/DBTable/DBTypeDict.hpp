/**
 * @file DBTypeDict.hpp
 * @author André Lucas Maegima
 * @brief
 * @version 0.2
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _DBTYPEDICT_HPP_
#define _DBTYPEDICT_HPP_

#include <list>
#include <iterator>
#include <unordered_map>
#include "Models/DBType/DBType.hpp"

class DBTypeDict {
   private:
    std::unordered_map<std::string, DBType *> types;

   public:
    DBTypeDict();
    DBTypeDict(std::list<DBType *> list);

    DBType *&operator[](std::string idx);
    bool contains(std::string name);
    std::unordered_map<std::string, DBType *>::iterator begin();
    std::unordered_map<std::string, DBType *>::iterator end();
};

inline DBType *&DBTypeDict::operator[](std::string idx) {
    return types[idx];
}

inline bool DBTypeDict::contains(std::string name) {
    return types.contains(name);
}

inline std::unordered_map<std::string, DBType *>::iterator DBTypeDict::begin() {
    return types.begin();
}

inline std::unordered_map<std::string, DBType *>::iterator DBTypeDict::end() {
    return types.end();
}

#endif  // _DBTYPEDICT_HPP_