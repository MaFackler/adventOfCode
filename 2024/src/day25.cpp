#include "advent.hpp"

enum class Type {
    LOCK,
    KEY
};

pair<vector<int>, Type> ParseLockOrKey(ifstream &fp) {
    vector<int> res;
    vector<string> lines;
    string line;
    for (size_t i = 0; i < 7; ++i) {
        getline(fp, line);
        // println("{}", line);
        lines.push_back(line);
    }

    for (size_t i = 0; i < 5; ++i) {
        int value = 0;
        for (size_t j = 0; j < lines.size(); ++j) {
            if (lines[j][i] ==  '#') {
                value++;
            }
        }
        res.push_back(value - 1);
    }

    auto t = lines[0][0] == '#' ? Type::LOCK : Type::KEY;
    return make_pair(res, t);
}

int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("../data/data25.txt");
    string line;
    vector<vector<int>> locks;
    vector<vector<int>> keys;
    do {
        auto [item, type] = ParseLockOrKey(fp);
        if (type == Type::LOCK) {
            locks.push_back(item);
        } else {
            keys.push_back(item);
        }
    } while (getline(fp, line));

    for (auto &lock : locks) {
        // println("Lock {}", lock);
        for (auto &key : keys) {
            auto zipped = views::zip(lock, key);
            bool correct = ranges::all_of(zipped, [](auto &&item) {
                return (std::get<0>(item) + std::get<1>(item)) < 6;
            });
            res1 += correct;
        }
    }

    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
