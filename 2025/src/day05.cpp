#include <catch2/catch_test_macros.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

using IngredientId = uint64_t;


struct Range {
    IngredientId from;
    IngredientId to;

    bool contains(IngredientId value) const {
        return from <= value && value <= to;
    }

    bool is_overlapping(const Range& other) const {
        Range first = *this;
        Range second = other;
        if (second.from < first.from) {
            swap(first, second);
        }

        return second.from >= first.from && second.from <= first.to;
    }

    void merge(Range second);
};

ostream& operator<<(ostream &out, const Range &a) {
    out << "Range{" << a.from << "/" << a.to << "}";
    return out;
}

void Range::merge(Range second) {
    Range first = *this;
    if (second.from < first.from) {
        swap(first, second);
    }
    Range res{
        .from=min(first.from, second.from),
        .to=max(first.to, second.to),
    };
    *this = res;
}



bool operator==(const Range& a, const Range& b) {
    return a.from == b.from && a.to == b.to;
}

bool operator<(const Range& a, const Range& b) {
    return a.from < b.from;
}

uint64_t part1(string &&filename) {
    fstream fp(filename);

    vector<Range> ranges;

    uint64_t p1 = 0;
    int part = 0;
    for (string line; getline(fp, line);) {
        stringstream ss(line);
        if (line.size() == 0) {
            part = 1;
            continue;
        }
        if (part == 0) {
            Range range = {};
            ss >> range.from;
            assert(ss.get() == '-');
            ss >> range.to;
            ranges.push_back(range);
        } else {
            IngredientId id = std::stoull(line);
            for (const auto &range : ranges) {
                if (range.contains(id)) {
                    p1++;
                    break;
                }
            }
        }
    }

    return p1;
}

uint64_t part2(string&& filename) {
    fstream fp(filename);
    vector<Range> ranges;
    for (string line; getline(fp, line);) {
        stringstream ss(line);

        if (line.size() == 0) {
            break;
        }
        Range range = {};
        ss >> range.from;
        assert(ss.get() == '-');
        ss >> range.to;
        ranges.push_back(range);
    }
    sort(ranges.begin(), ranges.end());
    for (int i = 0; i < ranges.size() - 1; ++i) {
        Range &a = ranges[i];
        const Range &b = ranges[i + 1];
        if (a.is_overlapping(b)) {
            a.merge(b);
            ranges.erase(ranges.begin() + i + 1);
            i--;
        }
    }
    uint64_t res = 0;
    for (auto &&range: ranges) {
        res += range.to - range.from + 1;
    }
    return res;
}

TEST_CASE("Range::overlapping") {
    Range a{0, 10};
    Range b{0, 10};

    REQUIRE(Range{10, 20}.is_overlapping(Range{0, 10}));
    REQUIRE(Range{10, 20}.is_overlapping(Range{0, 1}) == false);
    REQUIRE(Range{10, 20}.is_overlapping(Range{20, 30}));
    REQUIRE(Range{10, 20}.is_overlapping(Range{21, 30}) == false);
    REQUIRE(Range{10, 20}.is_overlapping(Range{0, 40}));
    REQUIRE(Range{0, 40}.is_overlapping(Range{20, 30}));
}

TEST_CASE("Range::merge") {
    Range a = Range{0, 10};
    a.merge({10, 20});
    REQUIRE(a == Range{0, 20});
    a = Range{10, 14};
    a.merge({12, 18});
    REQUIRE(a == Range{10, 18});
}


TEST_CASE("Example 1") {
    const auto p1 = part1("./data/day05e.txt");
    REQUIRE(p1 == 3);
    cout << p1 << "\n";
}

TEST_CASE("Solution 1") {
    const auto p1 = part1("./data/day05.txt");
    // REQUIRE(p1 == 3);
    cout << p1 << "\n";
}

TEST_CASE("Example 2") {
    const auto res = part2("./data/day05e.txt");
    REQUIRE(res == 14);
}

TEST_CASE("Solution 2") {
    const auto res = part2("./data/day05.txt");
    REQUIRE(res == 334714395325710);
    cout << res << "\n";
}
