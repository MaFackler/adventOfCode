#include <catch2/catch_test_macros.hpp>
#include <unordered_set>
#include <fstream>
#include <ranges>
#include <print>
#include <map>

using namespace std;

struct pair_hash {

    size_t operator()(const pair<int, int>& key) const {
        return key.first ^ key.second;
    }
};

using Beams = unordered_set<pair<int, int>, pair_hash>;
using Mirrows = unordered_set<pair<int, int>, pair_hash>;

int beams_advance(Beams &beams, const Mirrows &mirrows, size_t max_row) {
    Beams to_add;
    Beams to_remove;
    int res = 0;
    for (auto &beam : beams) {
        const auto moved_beam = make_pair(beam.first, beam.second + 1);
        if (moved_beam.second >= max_row) {
            to_remove.emplace(beam);
            continue;
        }
        if (mirrows.contains(moved_beam)) {
            to_add.emplace(make_pair(moved_beam.first - 1, moved_beam.second));
            to_add.emplace(make_pair(moved_beam.first + 1, moved_beam.second));
            to_remove.emplace(beam);
            res++;
        } else {
            to_add.emplace(moved_beam);
            to_remove.emplace(beam);
        }
    }
    beams.merge(to_add);
    for (const auto &ele : to_remove) {
        beams.erase(ele);
    }
    return res;
}

size_t count_dimensions(pair<int, int> beam, const Mirrows &mirrows, size_t max_row) {
    static map<pair<int, int>, size_t> dp;
    if (dp.contains(beam)) {
        return dp[beam];
    }
    if (beam.second >= max_row) {
        return 1;
    }

    size_t res = 0;
    if (mirrows.contains(beam)) {
        res += count_dimensions(make_pair(beam.first - 1, beam.second), mirrows, max_row);
        res += count_dimensions(make_pair(beam.first + 1, beam.second), mirrows, max_row);
    } else {
        res += count_dimensions(make_pair(beam.first, beam.second + 1), mirrows, max_row);
    }

    dp[beam] = res;

    return res;
}

tuple<Beams, Mirrows, size_t> read_beams_and_mirrows(string &&filename) {
    Beams beams;
    Mirrows mirrows;

    ifstream fp(filename);

    auto lines = ranges::istream_view<string>(fp)
        | ranges::to<vector<string>>();
    for (int y = 0; y < lines.size(); ++y) {
        auto &line = lines[y];
        for (int x = 0; x < line.size(); ++x) {
            char c = line[x];
            switch (c) {
                case '^': {
                    mirrows.emplace(make_pair(x, y));
                } break;
                case 'S': {
                    beams.emplace(make_pair(x, y));
                } break;
            }
        }
    }
    return {beams, mirrows, lines.size()};
}

int count_splits(Beams &beams, const Mirrows mirrows, size_t n) {
    int res = 0;
    while (beams.size() > 0) {
        res += beams_advance(beams, mirrows, n);
    }
    return res;
}


TEST_CASE("Example 1") {
    auto [beams, mirrows, n] = read_beams_and_mirrows("./data/day07e.txt");
    const auto res = count_splits(beams, mirrows, n);
    REQUIRE(res == 21);
}

TEST_CASE("Solution 1") {
    auto [beams, mirrows, n] = read_beams_and_mirrows("./data/day07.txt");
    const auto res = count_splits(beams, mirrows, n);
    println("{}", res);
}

TEST_CASE("Example 2") {
    auto [beams, mirrows, n] = read_beams_and_mirrows("./data/day07e.txt");
    REQUIRE(beams.size() == 1);
    const auto res = count_dimensions(*beams.begin(), mirrows, n);
    REQUIRE(res == 40);
}

TEST_CASE("Solution 2") {
    auto [beams, mirrows, n] = read_beams_and_mirrows("./data/day07.txt");
    REQUIRE(beams.size() == 1);
    const auto res = count_dimensions(*beams.begin(), mirrows, n);
    REQUIRE(res == 10357305916520);
    println("{}", res);
}
