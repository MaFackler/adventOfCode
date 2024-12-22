#include "advent.hpp"

using Pad = map<Pos, char>;

static set<pair<char, Pos>> DIRS = {
    {'<', Pos{-1, 0}},
    {'v', Pos{0, 1}},
    {'>', Pos{1, 0}},
    {'^', Pos{0, -1}},
};

set<string> FromTo(Pad &pad, char from, char to) {
    set<string> res;
    Pos start;
    bool found = false;
    for (auto &[key, value] : pad) {
        if (value == from) {
            start = key;
            found = true;
            break;
        }
    }
    assert(found);
    deque<tuple<Pos, string, size_t>> q = {{start, "", 0}};
    std::optional<size_t> maxSteps;

    while (!q.empty()) {
        auto [pos, path, steps] = q.front();
        q.pop_front();
        auto c = pad[pos];
        if (c == to) {
            path += 'A';
            if (!maxSteps.has_value()) {
                maxSteps = steps;
            }
            if (steps > maxSteps) {
                continue;
            }
            res.insert(path);
            continue;
        }

        if (maxSteps.has_value() && steps > maxSteps.value()) {
            continue;
        }

        for (auto [cdir, dir] : DIRS) {
            Pos npos = pos + dir;
            if (pad.contains(npos)) {
                q.push_back(make_tuple(npos, path + cdir, steps + 1));
            }
        }
    }
    return res;
}

set<pair<string, Pos>> PadGoto(Pad &pad, Pos p, char dest) {
    deque<tuple<Pos, string, size_t>> q = {{p, "", 0}};
    string res;
    Pos rpos;
    set<pair<string, Pos>> results;
    std::optional<size_t> maxSteps;

    while (!q.empty()) {
        auto [pos, path, steps] = q.front();
        q.pop_front();
        auto c = pad[pos];
        if (c == dest) {
            path += 'A';
            res = path;
            rpos = pos;

            if (!maxSteps.has_value()) {
                maxSteps = steps;
            }
            if (steps > maxSteps) {
                continue;
            }
            results.insert(make_pair(path, pos));
            continue;
        }

        if (maxSteps.has_value() && steps > maxSteps.value()) {
            continue;
        }

        for (auto [cdir, dir] : DIRS) {
            Pos npos = pos + dir;
            if (pad.contains(npos)) {
                q.push_back(make_tuple(npos, path + cdir, steps + 1));
            }
        }
    }
    // return make_pair(res, rpos);
    return results;
}

set<string> CalcPath(Pad &pad, Pos start, string input) {
    vector<pair<string, Pos>> paths = {
        {string(""), start},
    };
    for (char c : input) {
        vector<pair<string, Pos>> newPaths;
        for (auto &[path, start] : paths) {
            auto sequences = PadGoto(pad, start, c);
            for (auto &[subpath, pos] : sequences) {
                newPaths.push_back({path + subpath, pos});
            }
        }
        paths = newPaths;
    }
    set<string> res;
    for (auto &[path, _] : paths) {
        res.insert(path);
    }

    // auto mins = res
    //     | views::transform([](auto &&ele) { return ele.size(); });
    // size_t minlen = ranges::min(mins);
    // res = res
    //     | views::filter([&minlen](auto &&ele) { return ele.size() == minlen; })
    //     | ranges::to<set>();
    return res;
}



size_t Complexity(Pad &numeric, Pad &directional, string in) {
    Pos numericStart = {2, 3};
    Pos directionalStart = {2, 0};
    size_t minLen = std::numeric_limits<size_t>::max();
    for (auto &path1 : CalcPath(numeric, numericStart, in)) {
        // println("{} {}", path1.size(), path1);
        for (auto &path2 : CalcPath(directional, directionalStart, path1)) {
            // println("- {} {}", path2.size(), path2);
            for (auto &path3 : CalcPath(directional, directionalStart, path2)) {
                minLen = std::min(minLen, path3.size());
            }

        }
    }
    size_t value = std::stoull(in.substr(0, in.size() - 1));
    return minLen * value;
}




size_t GetCharCost(Pad &numeric, Pad &directional, Pad &pad, char from, char to, size_t depth) {

    static map<tuple<char, char, size_t>, size_t> cache;
    auto key = make_tuple(from, to, depth);
    if (pad == directional && cache.contains(key)) {
        return cache[key];
    }

    size_t res = 0;
    if (depth == 0) {
        auto sizes =  FromTo(directional, from ,to)
            | views::transform([](auto &&ele) { return ele.size(); });
        res = ranges::min(sizes);

    } else {

        auto paths = FromTo(pad, from, to);
        size_t mincost = std::numeric_limits<size_t>::max();
        for (auto path : paths) {
            size_t costs = 0;
            path = "A" + path;
            for (size_t i = 0; i < path.size() - 1; ++i) {
                auto pair = make_pair(path[i], path[i + 1]);
                costs += GetCharCost(numeric, directional, directional, pair.first, pair.second, depth - 1);
            }
            mincost = std::min(mincost, costs);
        }
        res = mincost;
    }
    if (pad == directional) {
        cache[key] = res;
    }
    return res;
}

size_t GetStringCost(Pad &numeric, Pad &directional, Pad &pad, string in, size_t depth) {
    size_t res = 0;
    char current = 'A';
    // NOTE: ignore last A
    assert (in[in.size() - 1] == 'A');
    for (size_t i = 0; i < in.size(); ++i) {
        char c = in[i];
        res += GetCharCost(numeric, directional, pad, current, c, depth);
        current = c;
    }
    return res;
}



int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("../data/day21.txt");
    vector<string> inputs;
    for (string line; getline(fp, line);) {
        inputs.push_back(line);
    }

    map<Pos, char> numeric;
    numeric[{0, 0}] = '7';
    numeric[{1, 0}] = '8';
    numeric[{2, 0}] = '9';
    numeric[{0, 1}] = '4';
    numeric[{1, 1}] = '5';
    numeric[{2, 1}] = '6';
    numeric[{0, 2}] = '1';
    numeric[{1, 2}] = '2';
    numeric[{2, 2}] = '3';
    // --
    numeric[{1, 3}] = '0';
    numeric[{2, 3}] = 'A';

    map<Pos, char> directional;
    // directional[{0, 0}] = '0';
    directional[{1, 0}] = '^';
    directional[{2, 0}] = 'A';
    directional[{0, 1}] = '<';
    directional[{1, 1}] = 'v';
    directional[{2, 1}] = '>';


    Pos numericStart = {2, 3};
    Pos directionalStart = {2, 0};
    for (auto &in : inputs) {
        // res1 += Complexity(numeric, directional, in);
        size_t value1 = std::stoull(in.substr(0, in.size() - 1));
        size_t cost1 = GetStringCost(numeric, directional, numeric, in, 2);
        res1 += cost1 * value1;

#if 1
        size_t value2 = std::stoull(in.substr(0, in.size() - 1));
        size_t cost2 = GetStringCost(numeric, directional, numeric, in, 25);
        res2 += cost2 * value2;
#endif
    }

    // println("{}", GetCharCost(directional, directional, 'A', '<', 0));
    
#if 0
+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
    | 0 | A |
    +---+---+


    <A^A>^^AvvvA
     0 2   9   A

    +---+---+
    | ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+

    <v<A>>^A<A>AvA<^AA>A<vAAA>^A
       <   A ^ A >  ^^ A  vvv  A
DEMO:
    v<<A>>^A<A>AvA<^AA>A<vAAA>^A
       <   A ^ A >


    <v<A>A<A>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A

       < v <   A >>


#endif


    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
