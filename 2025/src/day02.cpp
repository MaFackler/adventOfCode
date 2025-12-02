#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <print>
#include <fstream>
#include <cassert>
#include <ranges>


int64_t count_substring(std::string_view s, std::string_view substring) {
    int64_t res = 0;
    size_t pos = 0;
    while ((pos = s.find(substring, pos)) != std::string::npos) {
        res++;
        pos += substring.length();
    }
    return res;
}

bool is_valid(std::string_view number) {
    if (number.size() % 2 != 0) {
        return true;
    }
    const size_t n = number.size() / 2;
    std::string_view left = number.substr(0, n);
    std::string_view right = number.substr(n, number.size());
    if (left == right) {
        return false;
    }
    return true;
}

bool is_valid2(std::string_view number) {
    const size_t n = number.size() / 2;
    for (size_t i = 0; i < n; ++i) {
        std::string_view left = number.substr(0, i + 1);
        size_t nsubstrings = count_substring(number, left);
        if (nsubstrings * left.size() == number.size()) {
            return false;
        }
    }
    return true;
}

TEST_CASE("count_substring") {
    REQUIRE(count_substring("11", "1") == 2);
    REQUIRE(count_substring("1212", "12") == 2);
}

TEST_CASE("is_valid") {
    REQUIRE(is_valid("11") == false);
    REQUIRE(is_valid("12") == true);
    REQUIRE(is_valid("6464") == false);
    REQUIRE(is_valid("64641") == true);
}

TEST_CASE("is_valid2") {
    REQUIRE(is_valid2("11") == false);
    REQUIRE(is_valid2("12") == true);
    REQUIRE(is_valid2("6464") == false);
    REQUIRE(is_valid2("64641") == true);
    REQUIRE(is_valid2("12341234") == false);
    REQUIRE(is_valid2("123123123") == false);
    REQUIRE(is_valid2("1212121212") == false);
    REQUIRE(is_valid2("1111111") == false);
}

int64_t solve(std::string &&filename, std::function<bool(std::string_view)> func) {
    std::ifstream fp(filename);
    int64_t p1 = 0;
    for (std::string line; std::getline(fp, line);) {
        auto ranges =  std::views::split(line, ',')
            | std::views::transform([](auto &&ele) {
                return std::string_view(ele.begin(), ele.end());
            });
        for (auto &&range : ranges) {
            if (range.size() == 0) {
                continue;
            }
            auto pos = range.find('-');
            std::string_view left = std::string_view(range.begin(), pos);
            std::string_view right = std::string_view(range.begin() + pos + 1, range.end());
            int64_t start = 0;
            int64_t end = 0;
            std::from_chars(left.data(), left.data() + left.size(), start);
            std::from_chars(right.data(), right.data() + right.size(), end);

            assert(start < end);
            for (auto i = start; i <= end; ++i) {
                std::string as_string = std::to_string(i);
                if (!func(as_string)) {
                    p1 += i;
                }
            }
        }
    }
    return p1;
}


int64_t part1(std::string &&filename) {
    return solve(std::move(filename), is_valid);
}

int64_t part2(std::string &&filename) {
    return solve(std::move(filename), is_valid2);
}


TEST_CASE("Example 1") {
    const auto p1 = part1("./data/day02e.txt");
    REQUIRE(p1 == 1227775554);
    std::println("{}", p1);
}

TEST_CASE("Solution 1") {
    const auto p1 = part1("./data/day02.txt");
    REQUIRE(p1 == 38310256125);
}

TEST_CASE("Example 2") {
    const auto p2 = part2("./data/day02e.txt");
    REQUIRE(p2 == 4174379265);
}

TEST_CASE("Solution 2") {
    const auto p2 = part2("./data/day02.txt");
    REQUIRE(p2 == 58961152806);
    std::println("{}", p2);
}
