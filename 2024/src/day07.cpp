#include "advent.hpp"

typedef u64 (*Func)(u64, u64);
using Functions = vector<Func>;


u64 mul(u64 a, u64 b) {
    return a * b;
}

u64 add(u64 a, u64 b) {
    return a + b;
}

u64 combine(u64 a, u64 b) {
    assert(b > 0);
    a *= pow(10, ((int) std::log10(b) + 1));
    u64 res = a + b;
    return res;
}


void Calculate(vector<u64> numbers, Func func, u64 currentResult, std::vector<u64> &results, Functions &functions) {
    u64 next = numbers.front();
    numbers.erase(numbers.begin());
    currentResult = func(currentResult, next);
    if (numbers.size()) {
        for (auto &func : functions) {
            Calculate(numbers, func, currentResult, results, functions);
        }
    } else {
        results.push_back(currentResult);
    }
}

bool IsValid(vector<u64> numbers, u64 expectedResult, Functions &functions) {
    vector<u64> results;
    u64 start = numbers.front();
    numbers.erase(numbers.begin());
    for (auto &func : functions) {
        Calculate(numbers, func, start, results, functions);
    }
    return std::find(results.begin(), results.end(), expectedResult) != results.end();
}


int main() {
    auto fp = ifstream("data/day07.txt");

    u64 res1 = 0;
    u64 res2 = 0;

    Functions p1 = {mul, add};
    Functions p2 = {mul, add, combine};
    for (string line; getline(fp, line);) {
        istringstream stream(line);
        u64 expectedResult;
        stream >> expectedResult;
        char c = stream.get();
        assert(c == ':');
        c = stream.get();
        assert(c == ' ');
        string rest;
        std::getline(stream, rest);
        auto numbers = Split(rest, ' ')
            | std::ranges::views::transform([](auto ele) {
                return std::stoi(ele);
            })
            | std::ranges::to<vector<u64>>();
        if (IsValid(numbers, expectedResult, p1)) {
            res1 += expectedResult;
        }
        if (IsValid(numbers, expectedResult, p2)) {
            res2 += expectedResult;
        }
    }

    assert(combine(15, 6) == 156);
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
