#include <fstream>
#include <print>
#include <ranges>
#include <vector>
#include <numeric>
#include <tuple>
#include <algorithm>
#include <cassert>

using namespace std;

auto get_names_and_directions(string_view filepath) {
    auto fp = ifstream(filepath.data());

    string line;
    getline(fp, line);
    auto names = views::split(line, ',')
        | views::transform([](auto&& ele) {
            return string(ele.begin(), ele.end());
        })
        | ranges::to<vector>();

    getline(fp, line);
    getline(fp, line);
    auto directions = line
        | views::split(',')
        | views::transform([&](auto&& part) {
            int factor = part[0] == 'L' ? -1 : 1;
            int value = 0;
            string_view token(part.begin() + 1, part.end());
            from_chars(token.data(), token.data() + token.size(), value);
            return factor * value;
        })
        | ranges::to<vector<int>>();

    return tuple{names, directions};
}

void quest1() {
    auto [names, directions] = get_names_and_directions("./data/2025_q01_p1.txt");
    int max_value = names.size() - 1;
    auto part1lambda = [&max_value](int res, int ele) {
        res += ele;
        res = std::max(res, 0);
        res = std::min(res, max_value);
        return res;
    };
    auto offset1 = accumulate(directions.begin(),
                              directions.end(),
                              0, part1lambda);
    println("{}", names[offset1]);
}

void quest2() {
    auto [names, directions] = get_names_and_directions("./data/2025_q01_p2.txt");

    auto offset2 = accumulate(directions.begin(),
                              directions.end(),
                              0);

    println("{}", names[offset2 % names.size()]);
}

void quest3() {
    auto [names, directions] = get_names_and_directions("./data/2025_q01_p3.txt");
    for (auto&& direction : directions) {
        int offset = direction % (int) names.size();
        if (offset < 0) {
            offset = names.size() + offset;
        }
        
        swap(names[0], names[offset]);
    }

    println("{}", names[0]);

}


int main() {
    quest1();
    quest2();
    quest3();
}
