#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <sstream>
#include <map>
#include <ranges>
#include <print>

using namespace std;

inline size_t count_full(const string& line) {
    return ranges::count_if(line, [](char c) { return c == '#'; });
}

size_t part1(string&& filename) {
    fstream fp(filename);

    map<size_t, size_t> sizes;

    size_t p1 = 0;

    for (string line; getline(fp, line);) {
        stringstream ss(line);
        if (line.ends_with(":")) {
            size_t index = 0;
            size_t size = 0;
            ss >> index;
            ss.get();
            getline(fp, line);
            size += count_full(line);
            getline(fp, line);
            size += count_full(line);
            getline(fp, line);
            size += count_full(line);
            getline(fp, line);
            //println("{} {}", index, size);
            sizes[index] = size;
        } else {
            size_t dimx, dimy;
            char dummy;
            ss >> dimx >> dummy >> dimy;
            ss.get();
            ss.get();
            size_t size_space = dimx * dimy;
            string rest;
            getline(ss, rest);
            const auto presents = views::split(rest, ' ')
                | views::transform([](const auto &p) {
                    return stoll(string(p.begin(), p.end()));
                })
                | ranges::to<vector<size_t>>();

            const auto present_sizes = views::iota(0ul, presents.size())
                | views::transform([&sizes, &presents](auto index) {
                    return sizes[index] * presents[index]; 
                });

            const size_t size_presents = ranges::fold_left(present_sizes, size_t{0}, std::plus{});

            if (size_space > size_presents * 1.2f) {
                p1++;
            }


        }

    }
    return p1;
}

TEST_CASE("Example 1") {

    const auto res = part1("./data/day12e.txt");
    REQUIRE(res == 2);

}

TEST_CASE("Solution 1") {
    const auto res = part1("./data/day12.txt");
    REQUIRE(res == 579);
    println("{}", res);

}
