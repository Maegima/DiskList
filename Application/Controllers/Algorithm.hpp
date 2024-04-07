
/**
 * @file Algorithm.hpp
 * @author André Lucas Maegima
 * @brief Algorithm namespace definition
 * @version 0.4
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _ALGORITHM_HPP_
#define _ALGORITHM_HPP_

#include <list>
#include <vector>

namespace Algorithm {    
    template <typename T, typename K>
    bool contains(const std::list<T> &list, const K item) {
        return std::find(list.begin(), list.end(), item) != list.end();
    }

    template <typename T, typename K>
    bool contains(const std::vector<T> &list, const K item) {
        return std::find(list.begin(), list.end(), item) != list.end();
    }

    template <template<class> class T>
    T<std::string> split(const std::string str, const char delimiter) {
        size_t pos = 0;
        std::string token;
        std::string s = str;
        T<std::string> list;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            s.erase(0, pos + 1);
            list.push_back(token);
        }
        list.push_back(s);
        return list;
    }
}  // namespace Algorithm

#endif // _ALGORITHM_HPP_