#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <print>
#include <functional>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <queue>
#include <ranges>
#include <string>

using namespace std;

struct Machine {
    vector<bool> required;
    vector<vector<int>> buttons_list;
};

struct MachineQueued {
    size_t n;
    size_t button_index;
    vector<bool> state;
    // Machine machine;

    bool operator>(const MachineQueued &other) const {
        return n > other.n;
    }
};


void push_buttons(vector<bool> &state, const vector<int> &buttons) {
    for (int index : buttons) {
        state[index] = !state[index];
    }
}

int check_machine(const Machine &machine) {
    vector<bool> current(machine.required.size(), false);

    priority_queue<MachineQueued, vector<MachineQueued>, std::greater<>> queue;
    for (size_t i = 0; i < machine.buttons_list.size(); ++i) {
        queue.push(MachineQueued{
            .n=0,
            .button_index=i,
            .state=current,
        });
    }

    while (!queue.empty()) {
        auto item = queue.top();
        queue.pop();
        const vector<int> &buttons = machine.buttons_list[item.button_index];
        push_buttons(item.state, buttons);

        if (item.state == machine.required) {
            return item.n + 1;
            break;
        }

        for (size_t i = 0; i < machine.buttons_list.size(); ++i) {
            if (i != item.button_index) {
                queue.push(MachineQueued{
                    .n=item.n + 1,
                    .button_index=i,
                    .state=item.state,
                });
            }
        }

    }
    return 0;

}

int part1(string &&filename) {
    ifstream fp(filename);

    vector<Machine> machines;

    for (string line; getline(fp, line); ) {
        auto splitted = views::split(line, ' ')
            | ranges::to<vector<string>>();


        auto required = splitted[0].substr(1, splitted[0].size() - 2)
            | views::transform([](char c) { return c == '#'; })
            | ranges::to<vector<bool>>();

        auto buttons_list = splitted | views::drop(1) | views::take(splitted.size() - 2)
            | views::transform([](string &part) {
                vector<int> res;
                part = part.substr(1, part.length() - 2);
                for (auto number : views::split(part, ',')) {
                    res.push_back(std::stoi(string(number.begin(), number.end())));
                }
                return res;
            })
            | ranges::to<vector<vector<int>>>();

        machines.emplace_back(Machine{
            .required=required,
            .buttons_list=buttons_list,
        });
    }


    const auto res = ranges::fold_left(
        machines | views::transform(check_machine),
        0,
        std::plus{});
    return res;
}

TEST_CASE("Example 1") {
    const auto res = part1("./data/day10e.txt");
    REQUIRE(res == 7);
}

TEST_CASE("Solution 1") {
    const auto res = part1("./data/day10.txt");
    println("{}", res);
    REQUIRE(res == 396);
}
