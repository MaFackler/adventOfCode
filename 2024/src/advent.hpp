#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>  // std::sort
#include <numeric>   // std::accumulate
#include <ranges>  // zip
#include <cassert>
#include <cstdint>
#include <set>
#include <unordered_set>
#include <map>
#include <deque>

using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

using std::vector;
using std::map;
using std::set;
using std::unordered_set;
using std::string;
using std::cout;
using std::istringstream;
using std::sort;
using std::accumulate;
using std::ifstream;
namespace views = std::ranges::views;
namespace ranges = std::ranges;

template <typename T>
std::ostream& operator<<(std::ostream &os, std::vector<T> &v) {
    os << "[";
    for (auto it = v.begin(); it != v.end(); ++it) {
        os << *it;
        if (it < (v.end() - 1)) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream &os, std::unordered_set<T> &v) {
    os << "[";
    for (auto it = v.begin(); it != v.end(); ++it) {
        os << *it;
        if (it < (v.end() - 1)) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}


#if 0
template <typename T, typename U>
std::enable_if_t<is_iterable<T>::value, bool> Find(const T& container, U ele) {
    return std::find(container.begin(), container.end(), ele) != container.end();
}
#endif

vector<string> Split(string in, char delim) {
    vector<string> res;
    istringstream stream(in);
    string current;
    while (getline(stream, current, delim)) {
        res.push_back(current);
    }
    return res;
}

struct print {
    ~print() {
        std::cout << "\n";
    }
    print& operator<<(auto &ele) {
        std::cout << ele;
        return *this;
    }
};
