#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <fstream>
#include <functional>
#include <ranges>
#include <sstream>
#include <vector>
#include <iostream>
#include <print>

using namespace std;

using number = int64_t;

using Grid = vector<vector<number>>;
using Operators = vector<char>;

pair<Grid, Operators> parse_lines_to_grid_and_operators(string &&filename) {
    ifstream fp(filename);
    assert(fp.is_open());

    vector<string> lines;
    for (string line; getline(fp, line);) {
        lines.emplace_back(line);
    }
    auto grid = lines
        | views::take(lines.size() - 1)
        | views::transform([](auto &&line) {
            stringstream ss(line);
            return ranges::istream_view<number>(ss) | ranges::to<vector<number>>();
        })
        | ranges::to<Grid>();

    stringstream ss(lines.back());
    Operators operators = ranges::istream_view<char>(ss)
        | ranges::to<Operators>();
    return {grid, operators};
}

number calculate_sum(const vector<number>& numbers, char oper) {
    if (oper == '*') {
        return ranges::fold_left(numbers, number{1}, std::multiplies<>());
    }
    return ranges::fold_left(numbers, number{0}, std::plus<>());
}

number part2(string &&filename) {
    ifstream fp(filename);
    vector<string> lines;
    for (string line; getline(fp, line);) {
        lines.emplace_back(line);
    }

    char oper = 0;
    vector<number> numbers;
    number res = 0;
    for (int x = 0; x < lines[0].size(); ++x) {
        vector<char> chars;
        for (int y = 0; y < lines.size(); ++y) {
            if (x < lines.at(y).size()) {
                char c = lines.at(y).at(x);
                if (c != ' ') {
                    chars.push_back(c);
                }
            }
        }
        bool separator = chars.size() == 0;
        if (separator) {
            const auto diff = calculate_sum(numbers, oper);
            res += diff;
            numbers.clear();
            oper = 0;
        } else {
            char last = chars.back();
            if (last == '*' || last == '+') {
                oper = last;
                chars.pop_back();
            }
            numbers.emplace_back(stoll(string(chars.begin(), chars.end())));
        }
    }

    if (numbers.size()) {
        assert(oper != 0);
        res += calculate_sum(numbers, oper);
    }

    return res;
}

number part1(string &&filename) {
    auto [grid, operators] = parse_lines_to_grid_and_operators(std::move(filename));

    number res = 0;
    for (int x = 0; x < grid[0].size(); ++x) {
        vector<number> numbers;
        for (int y = 0; y < grid.size(); ++y) {
            numbers.push_back(grid.at(y).at(x));
        }
        res += calculate_sum(numbers, operators[x]);
    }

    return res;
}

TEST_CASE("Example 1") {
    const auto res = part1("./data/day06e.txt");
    REQUIRE(res == 4277556);
}

TEST_CASE("Solution 1") {
    const auto res = part1("./data/day06.txt");
    REQUIRE(res == 4693419406682);
    println("{}", res);

}

TEST_CASE("Example 2") {
    const auto res = part2("./data/day06e.txt");
    REQUIRE(res == 3263827);
}

TEST_CASE("Solution 2") {
    const auto res = part2("./data/day06.txt");
    REQUIRE(res == 9029931401920);
    println("{}", res);
}
