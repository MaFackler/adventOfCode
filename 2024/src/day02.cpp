#include "advent.hpp"


bool checkLevels(vector<int> levels) {
    auto differences = std::views::iota(1, int(levels.size()))
        | std::views::transform([&levels](int i) {
            return levels[i - 1] - levels[i];
        });

    auto increasing = std::ranges::all_of(differences, [](auto ele) { return ele >= 0; });
    auto decreasing = std::ranges::all_of(differences, [](auto ele) { return ele <= 0; });
    auto inrange = std::ranges::all_of(differences, [](auto ele) { return 1 <= abs(ele) && abs(ele) <= 3; });
    bool res = (increasing || decreasing) && inrange;
    return res;
}

int solve1(vector<vector<int>> &data) {

    int res = 0;
    for (auto &levels : data) {
        res += checkLevels(levels);
    }
    return res;
}


int solve2(vector<vector<int>> &data) {
    int res = 0;

    for (auto &d : data) {
        if (checkLevels(d)) {
            res++;
        } else {
            for (size_t i = 0; i < d.size(); ++i) {
                vector<int> copy(d);
                copy.erase(copy.begin() + i);
                if (checkLevels(copy)) {
                    res++;
                    break;
                }
            }
        }
    }
    return res;
}

int main() {
    auto fp = ifstream("data/day02.txt");
    
    vector<vector<int>> data;

    for (string line; getline(fp, line);) {
        auto splitted = Split(line, ' ')
            | views::transform([](auto ele) { return std::stoi(ele); })
            | ranges::to<vector>();
        data.push_back(splitted);
    }
    int res1 = solve1(data);
    int res2 = solve2(data);
    print() << "Solution1: " << res1;
    print() << "Solution2: " << res2;
    return 0;
}
