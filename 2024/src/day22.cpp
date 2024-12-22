#include "advent.hpp"


u64 mix(u64 a, u64 b) {
    u64 res = a ^ b;
    return res;
}

u64 prune(u64 a) {
    u64 res = a % 16777216;
    return res;
}

u64 idiv(double a, double b) {
    return (u64) (a / b);
}

u64 calculate(u64 secret) {
    secret = prune(mix(secret, secret * 64));
    secret = prune(mix(secret, idiv(secret, 32)));
    secret = prune(mix(secret, secret * 2048));
    return secret;
}

int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("../data/day22.txt");
    vector<u64> numbers;
    for (string line; getline(fp, line);) {
        numbers.push_back(std::stoll(line));
    }

    assert(mix(42, 15) == 37);
    assert(prune(100'000'000) == 16113920);

#if 0
    numbers.clear();
    numbers.push_back(1);
    numbers.push_back(2);
    numbers.push_back(3);
    numbers.push_back(2024);
    // numbers.push_back(123);
#endif


    res1 = 0;

    using MonkeySequence = tuple<i64, i64, i64, i64>;

    // map<i64, map<MonkeySequence, i64>> lookup2;
    set<MonkeySequence> sequences;

    vector<map<MonkeySequence, i64>> profits;

    auto out = std::ofstream("debug.txt");

    for (auto number : numbers) {
        vector<u64> values = {number % 10};
        auto &profit_ = profits.emplace_back();
        auto x = number;

        for (int i = 0; i < 2000; ++i) {
            x = calculate(x);
            values.push_back(x % 10);

            if (i > 4) {
                auto key = make_tuple(values[i - 2] - values[i - 3],
                                      values[i - 1] - values[i - 2],
                                      values[i - 0] - values[i - 1],
                                      values[i + 1] - values[i - 0]);
                if (!profit_.contains(key)) {
                    sequences.insert(key);
                    profit_[key] = x % 10;
                }
            }
        }

    }

    set<u64> results;

    for (auto &seq : sequences) {
        u64 res = 0;
        for (auto &profit : profits) {
            res += profit[seq];
        }
        results.insert(res);
    }
    res2 = ranges::max(results);




    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
