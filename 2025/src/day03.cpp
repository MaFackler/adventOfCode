#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <print>
#include <ranges>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

using number = int64_t;

number get_joltage(std::string_view line, const int digits) {
    auto as_list = line | views::transform([](const char &ele) {
        return (number) ele - '0';
    }) | ranges::to<vector>();

    number factor = pow(10, digits - 1);

    auto it_begin = as_list.begin();
    number res = 0;
    for (int i = 0; i < digits; ++i) {
        auto it = std::max_element(it_begin, as_list.end() - (digits - (i + 1)));
        res += *it * factor;
        it_begin = it + 1;
        factor /= 10;
    }
    return res;
}

number joltage_of_file(std::string filename, int digits) {
    ifstream fp(filename); 
    auto joltages = ranges::istream_view<string>(fp)
        | views::transform([&digits](auto &&ele) {
            return get_joltage(ele, digits);
        });;
    return ranges::fold_left(joltages, number{0}, std::plus<>());
}

number part1(std::string &&filename) {
    return joltage_of_file(std::move(filename), 2);
}

number part2(std::string &&filename) {
    return joltage_of_file(std::move(filename), 12);
}


TEST_CASE("get_joltage") {
    REQUIRE(get_joltage("987654321111111", 2) == 98);
    REQUIRE(get_joltage("811111111111119", 2) == 89);
    REQUIRE(get_joltage("234234234234278", 2) == 78);
    REQUIRE(get_joltage("818181911112111", 2) == 92);

    REQUIRE(get_joltage("987654321111111", 12) == 987654321111);
    REQUIRE(get_joltage("811111111111119", 12) == 811111111119);
    REQUIRE(get_joltage("234234234234278", 12) == 434234234278);
    REQUIRE(get_joltage("818181911112111", 12) == 888911112111);
}

TEST_CASE("Example 1") {
    const number p1 = part1("./data/day03e.txt");
    REQUIRE(p1 == 357);
}

TEST_CASE("Solution 1") {
    const number p1 = part1("./data/day03.txt");
    REQUIRE(p1 == 16973);
    println("{}", p1);
}

TEST_CASE("Example 2") {
    const number p2 = part2("./data/day03e.txt");
    REQUIRE(p2 == 3121910778619);
}

TEST_CASE("Solution 2") {
    const number p2 = part2("./data/day03.txt");
    REQUIRE(p2 == 168027167146027);
    println("{}", p2);
}
