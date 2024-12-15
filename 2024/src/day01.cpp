#include "advent.hpp"

i64 solve1(vector<int> left, vector<int> right) {
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    auto transformed = views::zip(left, right)
        | views::transform([](auto &&pair) { return abs(std::get<0>(pair) - std::get<1>(pair));
    });
    i64 res = std::ranges::fold_left(transformed, 0, std::plus{});

#if 0
    int res = 0;
    for (size_t i = 0; i < left.size(); ++i) {
        int distance = abs(right[i] - left[i]);
        res += distance;
    }
#endif
    return res;
}

i64 solve2(vector<int> left, vector<int> right) {

    i64 res = 0;
#if 0
    for (const auto &ele : left) {
        res += ele * std::count(right.begin(), right.end(), ele);
    }
#endif
    auto transformed = left
        | views::transform([&right](auto ele) { return ele * std::count(right.begin(), right.end(), ele); });
    res = ranges::fold_left(transformed, 0, std::plus{});
    return res;
}


int main() {
    auto fp = ifstream("../data/day01.txt");
    vector<int> left;
    vector<int> right;

    for (string line; getline(fp, line);) {
        istringstream stream(line);
        int lnumber, rnumber;
        stream >> lnumber;
        stream.ignore(3);
        stream >> rnumber;
        left.push_back(lnumber);
        right.push_back(rnumber);
    }
    i64 res1 = solve1(left, right);
    i64 res2 = solve2(left, right);
    println("Solution 1: {}", res1);
    println("Solution 2: {}", res2);
}
