#include "advent.hpp"


void PrintBoard(set<Pos> &walls, set<Pos> seen, Pos dim) {
    for (auto y : iota(0, dim.second + 1)) {
        for (auto x : iota(0, dim.first + 1)) {
            Pos p{x, y};
            char c = '.';
            if (walls.contains(p)) {
                c = '#';
            }
            if (seen.contains(p)) {
                c = 'O';
            }
            cout << c;
        }
        cout << "\n";
    }
}

std::optional<size_t> Walk(set<Pos> &walls, Pos start, Pos end, Pos dim) {
    deque<pair<Pos, size_t>> q = {make_pair(start, 0)};
    set<Pos> seen = {};

    auto inRange = [&dim](Pos p) {
        return 0 <= p.first && p.first <= dim.first && 0 <= p.second && p.second <= dim.second;
    };

    std::optional<size_t> res = std::nullopt;
    while (!q.empty()) {
        auto [cpos, cdistance] = q.front();
        q.pop_front();
        if (seen.contains(cpos)) {
            continue;
        }
        seen.insert(cpos);
        if (cpos == end) {
            res = cdistance;
            break;
        } else {
            for (auto dir : DIRECTIONS) {
                Pos npos = cpos + dir;
                if (inRange(npos) && !walls.contains(npos)) {
                    q.push_back(make_pair(npos, cdistance + 1));
                }
            }
        }

    }

    return res;
}

bool WalkPossible(vector<Pos> &bytes, Pos start, Pos end, Pos dim, size_t n) {
    set<Pos> walls;
    for (auto i : iota(0ul, n)) {
        walls.insert(bytes[i]);
    }
    auto res = Walk(walls, start, end, dim);
    return res.has_value();
}


int main() {

#if 0
    auto fp = ifstream("../data/day18e.txt");
    Pos dim = {6, 6};
    size_t n = 12;
#else
    auto fp = ifstream("../data/day18.txt");
    Pos dim = {70, 70};
    size_t n = 1024;
#endif
    vector<Pos> bytes;
    for (string line; getline(fp, line);) {
        int first, second;
        sscanf(line.c_str(), "%d,%d", &first, &second);
        bytes.push_back(Pos{first, second});
    }

    set<Pos> walls;
    for (int i = 0; i < n; ++i) {
        walls.insert(bytes[i]);
    }


    Pos end = dim;
    Pos start{0, 0};

    auto res1 = Walk(walls, start, end, dim).value();

    // Part2
    
    auto low = 0;
    auto high = bytes.size() - 1;
    while (low < high) {
        auto middle = (size_t) ((double)(low + high) / (double) 2);
        if (WalkPossible(bytes, start, end, dim, middle + 1)) {
            low = middle + 1;
        } else {
            high = middle;
        }
    }

    auto wrong = bytes[low];
    string res2 = std::format("{},{}", wrong.first, wrong.second);
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
