#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <ranges>
#include <string>
#include <iostream>
#include <print>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <map>
#include <utility>

using namespace std;

struct Position {
    int64_t x;
    int64_t y;
    bool operator==(const Position &b) const = default;
};

Position operator-(const Position a, const Position b) {
    return {
        .x=a.x - b.x,
        .y=a.y - b.y,
    };
}

template <>
struct std::hash<Position> {
    size_t operator()(const Position &pos) const {
        return std::hash<int64_t>{}(pos.x) ^ (std::hash<int64_t>{}(pos.y) << 1);
    }
};


using i64 = int64_t;

Position position_compress(Position a, const map<i64, i64> &xcoords, const map<i64, i64> &ycoords) {
    return Position{
        .x=xcoords.at(a.x),
        .y=ycoords.at(a.y),
    };
}

template <>
struct std::formatter<Position> : std::formatter<string> {
    auto format(const Position &pos, auto &ctx) const {
        return std::format_to(ctx.out(), "Position({}, {})", pos.x, pos.y);
    }
};


istream& operator>>(istream &out, Position &pos) {
    out >> pos.x;
    out.get();
    out >> pos.y;
    return out;
}

vector<Position> get_positions(string &&filename) {
    ifstream fp(filename);

    vector<Position> positions;
    for (string line; getline(fp, line);) {
        stringstream ss(line);
        Position pos;
        ss >> pos;
        positions.push_back(pos);
    }
    return positions;
}

pair<Position, Position> get_min_max_edge(const Position a, const Position b) {
    Position min_edge{
        .x=std::min(a.x, b.x),
        .y=std::min(a.y, b.y),
    };
    Position max_edge{
        .x=std::max(a.x, b.x),
        .y=std::max(a.y, b.y),
    };
    return {min_edge, max_edge};
}


int64_t part1(string &&filename) {
    vector<Position> positions = get_positions(std::move(filename));
    vector<int64_t> areas;
    for (int64_t i = 0; i < positions.size(); ++i) {
        for (int64_t j = i + 1; j < positions.size(); ++j) {
            Position a = positions[i];
            Position b = positions[j];
            int64_t dx = abs(a.x - b.x) + 1;
            int64_t dy = abs(a.y - b.y) + 1;
            int64_t area = dx * dy;
            areas.push_back(area);
        }
    }

    const int64_t res = *ranges::max_element(areas);
    return res;
}

bool grid_is_set(vector<vector<char>> &grid, const Position pos) {
    return grid[pos.y][pos.x] == '#';
}

void fill_grid(vector<vector<char>> &grid, const Position pos) {

    vector<Position> queue{pos};

    unordered_set<Position> seen;
    while (!queue.empty()) {
        const auto item = queue.back();
        queue.pop_back();
        grid[item.y][item.x] = '#';
        seen.insert(item);
        for (int64_t y = -1; y <= 1; ++y) {
            for (int64_t x = -1; x <= 1; ++x) {
                Position n{item.x + x, item.y + y};
                if (seen.contains(n)) {
                    continue;
                }
                if ((n.x >= 0 && n.x < grid[0].size() && n.y >= 0 && n.y < grid.size())
                    && !grid_is_set(grid, n)) {
                    queue.push_back(n);
                }
            }
        }
    }
}


map<i64, i64> build_coordinate_map(const vector<i64> &coords) {
    map<i64, i64> res;
    for (i64 i = 0; i < coords.size(); ++i) {
        res[coords[i]] = i * 2 + 1;
    }
    return res;
}

pair<vector<i64>, vector<i64>> get_x_and_y_coords(const vector<Position> &positions) {
    set<i64> xcoords;
    set<i64> ycoords;
    for (auto &pos : positions) {
        xcoords.insert(pos.x);
        ycoords.insert(pos.y);
    }

    vector<i64> xcoords_vec(xcoords.size());
    std::copy(xcoords.begin(), xcoords.end(), xcoords_vec.begin());
    vector<i64> ycoords_vec(ycoords.size());
    std::copy(ycoords.begin(), ycoords.end(), ycoords_vec.begin());
    return {xcoords_vec, ycoords_vec};
}


auto iter_compressed_edges(vector<vector<char>> &grid, const vector<Position> &positions,
                           const map<i64, i64> &xcoords_map, const map<i64, i64> &ycoords_map) {
    auto zipped_pairs = views::zip(positions,
                                   views::concat(positions | views::drop(1),
                                                 positions | views::take(1)))
                        | views::transform([&xcoords_map, &ycoords_map](auto &&t) {
                            return make_pair(position_compress(std::get<0>(t), xcoords_map, ycoords_map),
                                             position_compress(std::get<1>(t), xcoords_map, ycoords_map));
                        })
                        | views::transform([](auto &&t) {
                            const Position a = t.first;
                            const Position b = t.second;
                            return make_pair(Position{std::min(a.x, b.x), std::min(a.y, b.y)},
                                             Position{std::max(a.x, b.x), std::max(a.y, b.y)});
                        });
    return zipped_pairs;
                        
}

void set_greens_to_grid(vector<vector<char>> &grid, const vector<Position> &positions,
                        const map<i64, i64> &xcoords_map, const map<i64, i64> &ycoords_map) {
    for (auto [start, end] : iter_compressed_edges(grid, positions, xcoords_map, ycoords_map)) {
        for (i64 y = start.y; y <= end.y; ++y) {
            for (i64 x = start.x; x <= end.x; ++x) {
                grid[y][x] = '#';
            }
        }
    }
}

bool check_all_cells_set(vector<vector<char>> &grid, const Position start, const Position end) {
    for (i64 y = start.y; y <= end.y; ++y) {
        for (i64 x = start.x; x <= end.x; ++x) {
            if (!grid_is_set(grid, Position{x, y})) {
                return false;
            }
        }
    }
    return true;
}

int64_t part2(string &&filename, i64 start_dx, i64 start_dy) {
    auto positions = get_positions(std::move(filename));
    vector<Position> greens;


    auto [xcoords, ycoords] = get_x_and_y_coords(positions);

    auto xcoords_map = build_coordinate_map(xcoords);
    auto ycoords_map = build_coordinate_map(ycoords);

    i64 grid_x_dim = xcoords.size() * 2 + 1;
    i64 grid_y_dim = ycoords.size() * 2 + 1;

    vector<vector<char>> grid(grid_y_dim, vector<char>(grid_x_dim, '.'));

    for (auto &pos : positions) {
        const auto compressed = position_compress(pos, xcoords_map, ycoords_map);
        grid[compressed.y][compressed.x] = '#';
    }

    set_greens_to_grid(grid, positions, xcoords_map, ycoords_map);
    // Go one "in" and fill the grid from there
    Position start = position_compress(positions[0], xcoords_map, ycoords_map);
    start.x += start_dx;
    start.y += start_dy;
    fill_grid(grid, start);

#if 0
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[0].size(); ++x) {
            print("{}", grid[y][x]);
        }
        println("");
    }
#endif

    vector<int64_t> areas;
    for (int64_t i = 0; i < positions.size(); ++i) {
        for (int64_t j = i + 1; j < positions.size(); ++j) {
            Position a = position_compress(positions[i], xcoords_map, ycoords_map);
            Position b = position_compress(positions[j], xcoords_map, ycoords_map);
            auto [min_edge, max_edge] = get_min_max_edge(a, b);
            auto edge_right_top = Position{max_edge.x, min_edge.y};
            auto edge_left_bottom = Position{min_edge.x, max_edge.y};

            if (!grid_is_set(grid, min_edge)) {
                continue;
            }
            if (!grid_is_set(grid, max_edge)) {
                continue;
            }
            if (!grid_is_set(grid, edge_left_bottom)) {
                continue;
            }
            if (!grid_is_set(grid, edge_right_top)) {
                continue;
            }

            // NOTE: this can be optimized to just check the perimeter
            if (!check_all_cells_set(grid, min_edge, max_edge)) {
                continue;
            }

            int64_t dx = abs(positions[j].x - positions[i].x) + 1;
            int64_t dy = abs(positions[j].y - positions[i].y) + 1;
            areas.push_back(abs(dx) * abs(dy));
        }
    }
#define DEBUG_WRITE_PPM 0
#if DEBUG_WRITE_PPM

    ofstream out("debug.ppm");
    out << "P3\n";
    out << grid[0].size() << " " << grid.size() << " 255\n";
    for (i64 y = 0; y < grid.size(); ++y) {
        for (i64 x = 0; x < grid[0].size(); ++x) {
            char c = grid[y][x];
            int v = 0;
            if (c == '#') {
                out << "255 0 0 ";
            } else {
                out << "255 255 255 ";
            }
        }
        out << "\n";
    }
#endif
    return *ranges::max_element(areas);
}

TEST_CASE("Example 1") {
    const auto res = part1("./data/day09e.txt");
    REQUIRE(res == 50);
}

TEST_CASE("Solution 1") {
    const auto res = part1("./data/day09.txt");
    println("{}", res);
    REQUIRE(res == 4771508457);
}

TEST_CASE("Example 2") {
    const auto res = part2("./data/day09e.txt", 1, 1);
    REQUIRE(res == 24);
}

TEST_CASE("Soltion 2") {
    // NOTE: the offset has to be figured out manually
    const auto res = part2("./data/day09.txt", 1, -1);
    println("{}", res);
    REQUIRE(res == 1539809693);
}
