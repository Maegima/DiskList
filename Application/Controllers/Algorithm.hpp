
/**
 * @file Algorithm.hpp
 * @author your name (you@domain.com)
 * @brief Algorithm namespace definition
 * @version 0.1
 * @date 2024-03-23
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
}  // namespace Algorithm

#endif // _ALGORITHM_HPP_