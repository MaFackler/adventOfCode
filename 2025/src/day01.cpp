#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <string>
#include <fstream>
#include <print>
#include <cassert>

int64_t part1(std::string &&filename) {
    std::ifstream fp(filename);
    int64_t current_number = 50;
    int64_t res = 0;
    for (std::string line; getline(fp, line);) {
        int64_t factor = line[0] == 'L' ? -1 : 1;
        line = line.substr(1, line.size());
        int64_t number = std::stoll(line);
        current_number += number * factor;
        if (current_number % 100 == 0) {
            res++;
        }
        while (current_number < 0) {
            current_number += 100;
        }
        while (current_number >= 100) {
            current_number -= 100;
        }
    }
    return res;
}

std::pair<int64_t, int64_t> calculate_zeros(const int64_t number, const int64_t offset) {
    constexpr int dim = 100;
    int sign = offset > 0 ? 1 : -1;

    int64_t res = 0;
    for (int i = 1; i <= std::abs(offset); ++i) {
        const int64_t value = number + sign * i;
        if ((value % dim) == 0) {
            res++;
        }
    }
    int64_t new_number = (number + offset) % dim;
    if (new_number < 0) {
        new_number += dim;
    }
    return {res, new_number};
}

int64_t part2(std::string &&filename) {
    std::ifstream fp(filename);
    int64_t current_number = 50;
    int64_t res = 0;
    constexpr int dim = 100;
    for (std::string line; getline(fp, line);) {
        int64_t factor = line[0] == 'L' ? -1 : 1;
        line = line.substr(1, line.size());
        int64_t number = std::stoll(line);
        assert(number != 0);
        // std::cout << "Got number " << current_number << " and offset of " << number * factor << "\n";
        // int64_t old_number = current_number;
        auto result = calculate_zeros(current_number, number * factor);
        current_number = result.second;
        res += result.first;

        
    }
    return res;
}

TEST_CASE("calculate_zeros") {
    REQUIRE(calculate_zeros(10, -10) == std::make_pair(1, 0));
    REQUIRE(calculate_zeros(0, -1) == std::make_pair(0, 99));
    REQUIRE(calculate_zeros(0, -99) == std::make_pair(0, 1));
    REQUIRE(calculate_zeros(0, -100) == std::make_pair(1, 0));
    REQUIRE(calculate_zeros(0, -200) == std::make_pair(2, 0));

    REQUIRE(calculate_zeros(0, 1) == std::make_pair(0, 1));
    REQUIRE(calculate_zeros(0, 99) == std::make_pair(0, 99));
    REQUIRE(calculate_zeros(0, 100) == std::make_pair(1, 0));
    REQUIRE(calculate_zeros(0, 110) == std::make_pair(1, 10));
    REQUIRE(calculate_zeros(0, 200) == std::make_pair(2, 0));
    REQUIRE(calculate_zeros(0, 201) == std::make_pair(2, 1));
    REQUIRE(calculate_zeros(-1, 1) == std::make_pair(1, 0));
    REQUIRE(calculate_zeros(-1, 2) == std::make_pair(1, 1));
    REQUIRE(calculate_zeros(50, 1000) == std::make_pair(10, 50));
}

TEST_CASE("Day01::Part1 Example") {
    REQUIRE(part1("./data/day01e.txt") == 3);
}

TEST_CASE("Day01::Part2 Example") {
    REQUIRE(part2("./data/day01e.txt") == 6);
}

TEST_CASE("Print") {
    const auto p1 = part1("./data/day01.txt");
    REQUIRE(p1 == 1191);
    std::println("{}", p1);
    const auto p2 = part2("./data/day01.txt");
    REQUIRE(p2 == 6858);
    std::println("{}", p2);
}
