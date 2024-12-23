#include "advent.hpp"

using Network = map<string, set<string>>;
using Passwords = set<set<string>>;


void Search(Network &network, Passwords &passwords, string node, set<string> cur) {
    if (passwords.contains(cur)) {
        return;
    }
    passwords.insert(cur);
    for (auto &other : network[node]) {
        if (cur.contains(other)) {
            continue;
        }
        if (!ranges::all_of(cur, [&network, &other](string id) { return network[id].contains(other); })) {
            continue;
        }
        auto copy = cur;
        copy.insert(other);
        Search(network, passwords, other, copy);
    }
};

int main() {

    u64 res1 = 0;
    auto fp = ifstream("../data/day23.txt");
    Network network;
    set<string> computers;
    for (string line; getline(fp, line);) {
        string first, second;
        first = line.substr(0, 2);
        second = line.substr(3, 5);
        network[first].insert(second);
        network[second].insert(first);
        computers.insert(first);
        computers.insert(second);
    }

    int packsize = 3;
#if 0
    // NOTE: mine

    set<set<string>> packs;
    for (auto [key, values] : network) {
        vector<string> elements(values.begin(), values.end());
        elements.push_back(key);
        vector<bool> selection(elements.size(), false);
        std::fill(selection.begin(), selection.begin() + packsize, true);
        do {
            set<string> currentpack;
            for (int i = 0; i < elements.size(); ++i) {
                if (selection[i]) {
                    currentpack.insert(elements[i]);
                }
            }
            assert(currentpack.size() == packsize);
            packs.insert(currentpack);


        } while (std::prev_permutation(selection.begin(), selection.end()));
    }

    for (auto &pack : packs) {
        vector<string> asvec(pack.begin(), pack.end());

        bool correct = true;
        for (int i = 0; i < packsize; ++i) {
            auto key = asvec[i];
            for (int j = 0; j < packsize; ++j) {
                if (i == j) {
                    continue;
                }
                auto value = asvec[j];
                if (!network[key].contains(value)) {
                    correct = false;
                    break;
                }
            }
        }
        if (correct && (asvec[0][0] == 't' || asvec[1][0] == 't' || asvec[2][0] == 't')) {
            res1++;
            // println("{} {} {}", asvec[0], asvec[1], asvec[2]);
        }
    }
#endif

    for (auto [a, _] : network) {
        for (auto [b, _] : network) {
            for (auto [c, _] : network) {
                if (a < b && b < c && network[b].contains(a) && network[c].contains(a) && network[c].contains(b)) {
                    if (a.starts_with('t') || b.starts_with('t') || c.starts_with('t')) {
                        res1++;
                    }
                }
            }
        }
    }
        


    Passwords passwords;
    for (auto &[key, _] : network) {
        Search(network, passwords, key, {key});
    }

    size_t maxlen = ranges::max(passwords | views::transform([](auto &&ele) { return ele.size(); }));
    auto filtered = passwords
        | views::filter([&maxlen](auto &&ele) { return ele.size() == maxlen; })
        | views::transform([](auto &&ele) { return vector<string>(ele.begin(), ele.end()); })
        | ranges::to<vector>();

    assert(filtered.size() == 1);
    std::sort(filtered.begin(), filtered.end());

    string res2;
    for (size_t i = 0; i < filtered[0].size(); ++i) {
        res2 += filtered[0][i];
        if (i != filtered[0].size() - 1) {
            res2 += ",";
        }
    }

    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
