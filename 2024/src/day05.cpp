#include "advent.hpp"



int main() {
    auto fp = ifstream("../data/day05.txt");

    int res1 = 0;
    int res2 = 0;
    map<int, std::unordered_set<int>> rules;
    bool secondPart = false;
    for (string line; getline(fp, line);) {
        istringstream stream(line);
        if (!secondPart) {
            if (line == "") {
                secondPart = true;
                continue;
            }
            int left, right;
            stream >> left;
            char c = stream.get();
            assert (c == '|');
            stream >> right;
            rules[left].insert(right);
        } else {
            auto splitted = views::split(line, ',')
		| views::transform([](auto &&r) { return string(r.begin(), r.end()); })
		| ranges::to<vector>();
            auto values = splitted | std::ranges::views::transform([](auto ele) {
                return std::stoi(ele);
            });

            bool valid = std::ranges::all_of(values, [&rules, &values](int value) {
                auto i1 = std::find(values.begin(), values.end(), value);
                return std::ranges::all_of(rules[value], [&values, &i1](int ruleValue) {
                    auto i2 = std::find(values.begin(), values.end(), ruleValue);
                    return i1 <= i2;
                });
            });

            if (valid) {
                assert(values.size() % 2 == 1);
                int i = (int) (values.size() / 2);
                res1 += values[i];
            } else {
                auto ordered = values | std::ranges::to<vector<int>>();
                std::sort(ordered.begin(), ordered.end(), [&rules](int a, int b) {
                    return rules[a].contains(b);
                });
                res2 += ordered[(ordered.size() / 2)];
            }
        }
    }


    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
