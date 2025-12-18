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
#include <map>
#include <set>
#include <utility>

using namespace std;

using views::filter;
using ranges::to;

using Lights = set<int>;
using ButtonsList = vector<vector<int>>;
using Buttons = vector<int>;
using Joltages = vector<int>;
using Patterns = map<Lights, vector<ButtonsList>>;

struct Machine {
    Lights lights;
    ButtonsList buttons_list;
    Joltages joltages;
};

struct MachineQueued {
    size_t n;
    size_t button_index;
    Lights state;
    // Machine machine;

    bool operator>(const MachineQueued &other) const {
        return n > other.n;
    }
};


void push_buttons(Lights &state, const Buttons &buttons) {
    for (int index : buttons) {
        if (state.contains(index)) {
            state.erase(index);
        } else {
            state.insert(index);
        }
    }
}

// Old solution
int check_machine(const ButtonsList &buttons_list, const Lights &required) {

    priority_queue<MachineQueued, vector<MachineQueued>, std::greater<>> queue;
    for (size_t i = 0; i < buttons_list.size(); ++i) {
        queue.push(MachineQueued{
            .n=0,
            .button_index=i,
            .state=Lights{},
        });
    }

    while (!queue.empty()) {
        auto item = queue.top();
        queue.pop();
        const vector<int> &buttons = buttons_list[item.button_index];
        push_buttons(item.state, buttons);

        if (item.state == required) {
            return item.n + 1;
            break;
        }

        for (size_t i = 0; i < buttons_list.size(); ++i) {
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


vector<Machine> parse_machines(string &&filename) {
    ifstream fp(filename);
    vector<Machine> machines;

    for (string line; getline(fp, line); ) {
        auto splitted = views::split(line, ' ')
            | ranges::to<vector<string>>();


        auto first = splitted[0].substr(1, splitted[0].size() - 2);
        auto lights = views::iota(0ul, first.size())
            | filter([&first](size_t i) { return first[i] == '#'; })
            | to<Lights>();

        auto buttons_list = splitted | views::drop(1) | views::take(splitted.size() - 2)
            | views::transform([](string &part) {
                vector<int> res;
                part = part.substr(1, part.length() - 2);
                for (auto number : views::split(part, ',')) {
                    res.push_back(std::stoi(string(number.begin(), number.end())));
                }
                return res;
            })
            | to<vector<vector<int>>>();

        string last = splitted[splitted.size() - 1];
        auto truncated = last | views::drop(1) | views::take(last.size() - 2);
        auto joltages = views::split(truncated, ',')
            | views::transform([](const auto &&part) {
                return std::stoi(string(part.begin(), part.end()));
            })
            | to<Joltages>();


        machines.emplace_back(Machine{
            .lights=lights,
            .buttons_list=buttons_list,
            .joltages=joltages,
        });
    }
    return machines;

}

template<typename T, typename F>
void combinations(const vector<T> sequence, size_t n, F&& f) {
    vector<bool> mask(sequence.size());
    std::fill(mask.begin(), mask.begin() + n, true);
    vector<T> permu;

    do {
        permu.clear();
        for (size_t i = 0; i < sequence.size(); ++i) {
            if (mask[i]) {
                permu.push_back(sequence[i]);
            }
        }
        f(permu);
    } while (std::prev_permutation(mask.begin(), mask.end()));
}

auto valid_patterns(const Lights& required, const ButtonsList& buttons_list) -> Patterns {
    Patterns res;
    for (size_t i = 0; i <= buttons_list.size(); ++i) {
        combinations(buttons_list, i, [&i, &res](const ButtonsList& presses) {
            auto state = Lights{};
            for (const Buttons &buttons : presses) {
                push_buttons(state, buttons);
            }
            res[state].push_back(presses);
        });
    }

    return res;
}

size_t part1(string &&filename) {
    const auto machines = parse_machines(std::move(filename));
    size_t res = 0;
    for (const Machine &machine: machines) {
        Patterns patterns = valid_patterns(machine.lights, machine.buttons_list);
        assert (patterns.contains(machine.lights));
        vector<ButtonsList> &all_presses = patterns.at(machine.lights);

        res += ranges::min(all_presses | views::transform([](const ButtonsList &ele) { return ele.size(); }));
    }
    return res;
}


auto get_lights_from_joltages(const Joltages& joltages) -> Lights {
    const auto res = views::iota(0ul, joltages.size())
        | views::filter([&joltages](size_t index) { return joltages[index] % 2 == 1; })
        | to<Lights>();
    return res;
}

optional<size_t> get_min_presses(const Joltages& joltages, const Patterns& patterns) {
    if (ranges::all_of(joltages, [](int value) { return value == 0; })) {
        return 0;
    }
    const Lights joltage_lights = get_lights_from_joltages(joltages);       
    optional<size_t> res;

    if (!patterns.contains(joltage_lights)) {
        return res;
    }

    for (auto &presses : patterns.at(joltage_lights)) {
        Joltages joltages_after = joltages;
        for (const Buttons &buttons : presses) {
            for (int index : buttons) {
                joltages_after[index] -= 1;
            }
        }

        if (ranges::any_of(joltages_after, [](int value) { return value < 0; })) {
            continue;
        }
        for (int i = 0; i < joltages_after.size(); ++i) {
            assert(joltages_after[i] % 2 == 0);
            joltages_after[i] = joltages_after[i] / 2;
        }

        optional<size_t> sub_result = get_min_presses(joltages_after, patterns);
        if (!sub_result.has_value()) {
            continue;
        }

        const size_t num_presses = presses.size() + 2 * sub_result.value();
        if (res.has_value()) {
            res = std::min(num_presses, res.value());
        } else {
            res = num_presses;
        }

    }

    return res;
}

size_t part2(string &&filename) {
    const auto machines = parse_machines(std::move(filename));

    size_t res = 0;
    for (const Machine &machine : machines) {
        const Patterns& patterns = valid_patterns(machine.lights, machine.buttons_list);
        res += get_min_presses(machine.joltages, patterns).value();
    }
    return res;
}

TEST_CASE("Example 1") {
    const size_t res = part1("./data/day10e.txt");
    REQUIRE(res == 7);
}
TEST_CASE("Solution 1") {
    const size_t res = part1("./data/day10.txt");
    println("{}", res);
    REQUIRE(res == 396);
}

// NOTE: solution 2 was taken from
// https://aoc.winslowjosiah.com/solutions/2025/day/10/
// https://old.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/

TEST_CASE("Example 2") {
    const size_t res = part2("./data/day10e.txt");
    REQUIRE(res == 33);
}

TEST_CASE("Solution 2") {
    const size_t res = part2("./data/day10.txt");
    REQUIRE(res == 15688);
    println("{}", res);
}
