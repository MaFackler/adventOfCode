#include "advent.hpp"



int main() {
    auto fp = ifstream("data/day05.txt");

    int res1 = 0;
    int res2 = 0;
    map<int, vector<int>> rules;
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
            rules[left].push_back(right);
        } else {
            vector<string> splitted = Split(line, ',');
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
                int i = (values.size() / 2);
                res1 += values[i];
            } else {
                vector<int> ordered;
                for (int value : values) {
                    auto index = ordered.end();
                    for (int ruleValue : rules[value]) {
                        if (std::find(values.begin(), values.end(), ruleValue) != values.end()) {
                            index = std::min(index, std::find(ordered.begin(), ordered.end(), ruleValue));
                        }
                    }
                    ordered.insert(index, value);
                }
                res2 += ordered[(ordered.size() / 2)];
            }
        }
    }


    print() << "Solution1: " << res1;
    print() << "Solution2: " << res2;
    return 0;
}
