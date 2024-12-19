#include "advent.hpp"


size_t getcount(vector<string> &towels, string word) {
    static map<string, size_t> DP;
    if (DP.contains(word)) {
        return DP[word];
    }
    size_t res = 0;
    if (word.size() == 0) {
        res = 1;
    } else {
        for (auto &towel : towels) {
            if (word.starts_with(towel)) {
                res += getcount(towels, word.substr(towel.size(), word.size() - towel.size()));
            }
        }
    }
    DP[word] = res;
    return DP[word];
}

int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("../data/day19.txt");
    string line;
    getline(fp, line);
    auto towels = views::split(line, ',')
        | views::transform([](auto &&range) {
            string res(range.begin(), range.end());
            res.erase(std::remove(res.begin(), res.end(), ' '), res.end());
            return res;
        })
        | ranges::to<vector>();
    getline(fp, line);
    assert(line == "");

#if 0
    for (; getline(fp, line);) {
        assert(line.size() > 0);

        deque<tuple<string, size_t, string>> q;
        for (auto &towel : towels) {
            if (line.substr(0, towel.size()) == towel) {
                q.push_back(make_tuple(towel, towel.size(), towel));
            }
        }

        int found = false;
        while (!q.empty()) {
            auto [towel, index, s] = q.front();
            q.pop_front();
            if (s == line) {
                found = true;
                break;
            }
            for (auto &ntowel : towels) {
                if (line.substr(index, ntowel.size()) == ntowel) {
                    q.push_front(make_tuple(ntowel, index + ntowel.size(), s + ntowel));
                }
            }
        }
        if (found) {
            res1++;
        }

    }
#endif
    for (;getline(fp, line);) {
        assert(line.size() > 0);
        size_t n = getcount(towels, line);
        res1 += n > 0 ? 1 : 0;
        res2 += n;
    }
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
