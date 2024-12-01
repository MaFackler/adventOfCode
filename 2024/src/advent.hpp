#pragma once
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

using std::vector;
using std::string;
using std::cout;
using std::istringstream;
using std::sort;
using std::accumulate;
using std::ifstream;
using std::ranges::views::transform;
using std::ranges::views::zip;
using std::ranges::fold_left;

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

struct print {
    ~print() {
        std::cout << "\n";
    }
    print& operator<<(auto &ele) {
        std::cout << ele;
        return *this;
    }
};
