#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <ranges>
#include <vector>
#include <print>
#include <cassert>

using namespace std;


using Grid = vector<vector<bool>>;

vector<vector<bool>> file_to_vector(string && filename) {
    ifstream fp(filename);
    auto res = ranges::istream_view<string>(fp)
        | views::transform([](auto &&line) {
            return line | views::transform([](char c) {
                assert(c == '.' || c == '@');
                return c == '@';
            });
        })
        | ranges::to<Grid>();
    return res;
}

pair<int, Grid> get_number(const Grid &grid) {
    int res = 0;
    Grid resulting_grid = grid;
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (!grid[y][x]) {
                continue;
            }
            int neighbours = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if ((dy != 0 || dx != 0)
                        && y + dy >= 0 && y + dy < grid.size()
                        && x + dx >= 0 && x + dx < grid[0].size()){
                        neighbours += (int) grid[y + dy][x + dx];
                    }
                }
            }
            if (neighbours < 4) {
                res++;
                resulting_grid[y][x] = false;
            }
        }
    }
    return {res, resulting_grid};
}


TEST_CASE("Example 1") {
    int p1 = get_number(file_to_vector("./data/day04e.txt")).first;
    REQUIRE(p1 == 13);
}

TEST_CASE("Solution 1") {
    int p1 = get_number(file_to_vector("./data/day04.txt")).first;
    REQUIRE(p1 == 1491);
    println("{}", p1);
}

int part2(string &&filename) {
    auto grid = file_to_vector(std::move(filename));
    int p2 = 0;
    for (;;) {
        auto [removed, new_grid] = get_number(grid);
        if (removed == 0) {
            break;
        }
        p2 += removed;
        grid = new_grid;
    }
    return p2;
}

TEST_CASE("Example 2") {
    const int p2 = part2("./data/day04e.txt");
    REQUIRE(p2 == 43);
}

TEST_CASE("Solution 2") {
    const int p2 = part2("./data/day04.txt");
    println("{}", p2);
    REQUIRE(p2 == 8722);
}
