#include <algorithm>
#include <bits/ranges_algo.h>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <execution>
#include <format>
#include <fstream>
#include <functional>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>
#include <print>
#include <unordered_set>

using namespace std;

struct Position {
    int64_t x;
    int64_t y;
    int64_t z;

    float distance(Position other) const {
        const auto dx = other.x - x;
        const auto dy = other.y - y;
        const auto dz = other.z - z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }
};

template<>
struct std::formatter<Position> : std::formatter<string> {
    auto format(const Position &pos, auto &ctx) const {
        return std::format_to(ctx.out(), "Position({}, {}, {})",
                              pos.x, pos.y, pos.z);
    }
};



bool operator==(const Position &a, const Position &b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

istream& operator>>(istream &stream, Position &pos) {
    stream >> pos.x;
    stream.get();
    stream >> pos.y;
    stream.get();
    stream >> pos.z;
    return stream;
}

pair<Position, Position> get_closest_pair(const vector<Position> &positions) {
    std::optional<pair<Position, Position>> res;
    float min_distance = 0;
    for (auto [a, b] : views::cartesian_product(positions, positions)) {
        if (a == b) {
            continue;
        }
        float distance = a.distance(b);
        if (!res.has_value() || distance < min_distance) {
            min_distance = distance;
            res = {a, b};
        }
    }
    assert(res.has_value());
    return res.value();
}


vector<pair<Position, Position>> get_closest_pairs(const vector<Position> &positions) {

    vector<pair<Position, Position>> res;
    for (int i = 0; i < positions.size(); ++i) {
        for (int j = i + 1; j < positions.size(); ++j) {
            res.push_back(make_pair(positions[i], positions[j]));
        }
    }

    ranges::sort(res, [](const pair<Position, Position> &a,
                               const pair<Position, Position> &b) -> bool {
        return a.first.distance(a.second) < b.first.distance(b.second);
    });
    return res;
}

vector<Position> read_positions(string &&filename) {
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

void connect(const Position a, const Position b, vector<vector<Position>> &circuits) {
    optional<int> ia;
    optional<int> ib;
    for (int i = 0; i < circuits.size(); ++i) {
        auto &circuit = circuits[i];
        if (ranges::find(circuit, a) != circuit.end()) {
            assert(!ia.has_value());
            ia = i;
        }
        if (ranges::find(circuit, b) != circuit.end()) {
            assert(!ib.has_value());
            ib = i;
        }
    }
    if (ia.has_value() && ib.has_value()) {
        if (ia.value() == ib.value()) {
            // same circuit do nothing
        } else {
            // connect spearate circuits
            auto &circuits_a = circuits[ia.value()];
            auto &circuits_b = circuits[ib.value()];
            circuits_a.insert(circuits_a.end(), circuits_b.begin(), circuits_b.end());
            circuits.erase(circuits.begin() + ib.value());
        }
    } else if (ia.has_value()) {
        circuits.at(ia.value()).push_back(b);
    } else if (ib.has_value()) {
        circuits.at(ib.value()).push_back(a);
    } else {
        assert(!ia.has_value() && !ib.has_value());
        vector<Position> new_circuit{
            a, b
        };
        circuits.push_back(new_circuit);
    }
}

size_t part1(string &&filename, size_t n) {
    const auto positions = read_positions(std::move(filename));

    vector<vector<Position>> circuits;
    auto pairs_sorted = get_closest_pairs(positions);

    int connected = 0;
    for (auto [a, b] : pairs_sorted) {
        connect(a, b, circuits);
        connected++;  
        if (connected == n) {
            break;
        }
    }

    auto sizes = circuits
        | views::transform([](const auto &circuit) {
            return circuit.size();
        })
        | ranges::to<vector<size_t>>();

    ranges::sort(sizes, std::greater<>());

    auto scope = sizes | views::take(3);
    const auto res = ranges::fold_left(scope, 1, std::multiplies<>());
    return res;
}

size_t part2(string &&filename) {
    const auto positions = read_positions(std::move(filename));
    size_t n = positions.size();
    auto pairs = get_closest_pairs(positions);
    vector<vector<Position>> circuits;
    for (auto [a, b] : pairs) {
        connect(a, b, circuits);
        for (auto &circuit : circuits) {
            if (circuit.size() == n) {
                return a.x * b.x;
            }
        }
    }
    return 0;
}


TEST_CASE("Example 1") {
    const auto res = part1("./data/day08e.txt", 10);
    REQUIRE(res == 40);
}

TEST_CASE("Solution 1") {
    const auto res = part1("./data/day08.txt", 1000);
    // REQUIRE(res == 40);
    println("{}", res);
}

TEST_CASE("Example 2") {
    const auto res = part2("./data/day08e.txt");
    REQUIRE(res == 25272);
}

TEST_CASE("Solution 2") {
    const auto res = part2("./data/day08.txt");
    println("{}", res);
    REQUIRE(res == 8663467782);
}
