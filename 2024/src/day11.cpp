#include "advent.hpp"

using Stones = vector<u64>;
using Stone = u64;

void Blink(Stones &stones) {
    for (int i = 0; i < stones.size(); ++i) {
        Stone &stone = stones[i];
        int n = log10(stone) + 1;
        if (stone == 0) {
            stone = 1;
        } else if ((n % 2) == 0) {
            string s = std::to_string(stone);
            string left = s.substr(0, s.size() / 2);
            string right = s.substr(s.size() / 2, s.size());
            stones[i] = std::stoi(left);
            stones.insert(stones.begin() + i + 1, std::stoi(right));
            i++;
        } else {
            stone *= 2024;
        }
    }
}

int Solve1(Stones stones) {
    for (auto _ : views::iota(0, 25)) {
        Blink(stones);
    }
    return stones.size();
}

u64 SolveStone(Stone stone, size_t n) {
    static map<std::tuple<Stone, size_t>, u64> cache;
    auto key = std::make_tuple(stone, n);
    if (cache.contains(key)) {
        return cache[key];
    }
    int digits = log10(stone) + 1;
    u64 res = 0;
    if (n == 0) {
        res = 1;
    } else if (stone == 0) {
        res = SolveStone(1, n - 1);
    } else if ((digits % 2) == 0) {
        string s = std::to_string(stone);
        string left = s.substr(0, s.size() / 2);
        string right = s.substr(s.size() / 2, s.size());
        res = SolveStone(std::stoull(left), n - 1) + SolveStone(std::stoull(right), n - 1);
    } else {
        res = SolveStone(stone * 2024, n - 1);
    }

    cache[key] = res;
    return res;
}

u64 Solve2(Stones stones, size_t n) {
    u64 res = 0;
    for (Stone stone : stones) {
        res += SolveStone(stone, n);
    }
    return res;
}

int main() {
    auto fp = ifstream("data/day11.txt");

    u64 res1 = 0;
    u64 res2 = 0;
    Stones stones;
    for (string line; getline(fp, line);) {
        stones = Split(line, ' ')
            | views::transform([](auto ele) { return std::stoi(ele); })
            | ranges::to<Stones>();
        break;
    }
    // res1 = Solve1(stones);
    res1 = Solve2(stones, 25);
    res2 = Solve2(stones, 75);
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
