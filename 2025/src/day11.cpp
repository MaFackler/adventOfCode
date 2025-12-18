#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <print>
#include <ranges>
#include <map>
#include <deque>
#include <bitset>
#include <utility>
#include <set>

using namespace std;

using views::split;
using ranges::to;


auto parse_lookup_map(string &&filename) -> map<string, vector<string>> {
    ifstream fp(filename);

    map<string, vector<string>> lookup;

    for (string line; getline(fp, line);) {
        string left;
        stringstream ss(line);
        getline(ss, left, ':');
        ss.get();
        string right;
        getline(ss, right);
        
        const auto destinations = split(right, ' ')
            | to<vector<string>>();
        lookup[left] = destinations;
    }
    return lookup;
}

size_t part1(string&& filename) {
    const auto lookup = parse_lookup_map(std::move(filename));

    deque<string> q{"you"};
    size_t res = 0;
    while (!q.empty()) {
        auto &item = q.front();

        if (item == "out") {
            res++;
        } else {
            for (auto &child : lookup.at(item)) {
                q.push_back(child);
            }
        }
        q.pop_front();
    }

    return res;
}

struct QueueItem {
    string key;
    bitset<2> mask;
    set<string> seen;
};

static map<tuple<string, bool, bool>, size_t> DP;

size_t get_number(string key, const map<string, vector<string>>& lookup, bool dac_found, bool fft_found) {
    auto dp_key = make_tuple(key, dac_found, fft_found);
    if (DP.contains(dp_key)) {
        return DP.at(dp_key);
    }
    if (key == "out") {
        return dac_found && fft_found ? 1 : 0;
    } else if (key == "dac") {
        dac_found = true;
    } else if (key == "fft") {
        fft_found = true;
    }
    size_t res = 0;
    for (const string &child : lookup.at(key)) {
        res += get_number(child, lookup, dac_found, fft_found);
    }

    DP[dp_key] = res;
    return res;
}


TEST_CASE("Example 1") {
    const size_t res = part1("./data/day11e.txt");
    REQUIRE(res == 5);
}

TEST_CASE("Solution 1") {
    const size_t res = part1("./data/day11.txt");
    println("{}", res);
    REQUIRE(res == 477);
}

TEST_CASE("Example 2") {
    DP.clear();
    const auto lookup = parse_lookup_map("./data/day11ee.txt");
    const size_t res = get_number("svr", lookup, false, false);
    REQUIRE(res == 2);
}

TEST_CASE("Solution 2") {
    DP.clear();
    const auto lookup = parse_lookup_map("./data/day11.txt");
    const size_t res = get_number("svr", lookup, false, false);
    println("{}", res);
    REQUIRE(res == 383307150903216);
}
