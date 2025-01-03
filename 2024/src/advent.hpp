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
#include <cstdlib>
#include <cstdint>
#include <set>
#include <unordered_set>
#include <map>
#include <deque>
#include <print>
#include <stack>
#include <queue>
#include <limits>
#include <variant>


using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

#ifdef _MSC_VER
#define sscanf sscanf_s
#endif
using std::stack;
using std::deque;
using std::println;
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
using std::priority_queue;
using std::pair;
using std::make_pair;
using std::tuple;
using std::make_tuple;
namespace views = std::ranges::views;
using views::iota;
namespace ranges = std::ranges;
using std::variant;

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

// Grid stuff
using Grid = vector<string>;
using Pos = std::pair<int, int>;

static vector<Pos> DIRECTIONS = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1},
};

inline Pos RotateClockwise(Pos p) {
    return Pos{p.second, -p.first};
}

inline Pos RotateCounterClockwise(Pos p) {
    return Pos{-p.second, p.first};
}

inline Pos operator+(Pos a, Pos b) {
    return {a.first + b.first, a.second + b.second};
}

inline Pos operator-(Pos a, Pos b) {
    return {a.first - b.first, a.second - b.second};
}

inline Pos operator*(Pos a, int value) {
    return {a.first * value, a.second * value};
}

inline bool operator==(Pos a, Pos b) {
    return a.first == b.first && a.second == b.second;
}

inline float DistanceSquared(Pos a, Pos b) {
    Pos diff = b - a;
    return diff.first * diff.first + diff.second * diff.second;
}

inline bool InRange(Grid &grid, Pos p) {
    return p.first >= 0 && p.second >= 0 && p.first < grid[0].size() && p.second < grid.size();
}

inline auto IterGrid(const Grid& grid) {
    auto yrange = views::iota(0UL, grid.size());
    auto xrange = views::iota(0UL, grid[0].size());
    auto nested = views::cartesian_product(yrange, xrange);

    auto iter = nested
        | views::transform([&grid](const auto& pair) {
            auto [y, x] = pair; // Unpack the tuple
            return std::make_tuple(y, x, grid[y][x]);
        });
    return iter;
};

namespace std {
    template<>
    struct hash<Pos> {
        std::size_t operator()(const Pos &p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };
}

// Algorihtms

auto iter_pairs(auto &cont) {
    auto iter = views::iota(0ul, cont.size() / 2)
        | views::transform([&cont](auto i) {
            auto first = cont[i * 2];
            auto second = cont[i * 2 + 1];
            return std::make_pair(first, second); 
        });

    return iter;
}
