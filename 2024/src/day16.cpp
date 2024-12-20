#include "advent.hpp"
using Walls = unordered_set<Pos>;

size_t R = 0;
size_t C = 0;

struct Path {
    Pos pos;
    Pos dir;
    size_t score;

    bool operator==(const Path &other) const {
        return pos == other.pos && dir == other.dir && score == other.score;
    }
};

namespace std {
    template<>
    struct hash<Path> {
        size_t operator()(const Path &p) const {
            return std::hash<Pos>()(p.pos) + (std::hash<Pos>()(p.dir) << 1) + (std::hash<size_t>()(p.score) << 2);
        }
    };
}

struct ComparePath {
    bool operator()(const Path &a, const Path &b) {
        return a.score > b.score;
    }
};


pair<size_t, size_t> Walk(Walls &walls, Pos pos, Pos dir, Pos end) {
    priority_queue<Path, vector<Path>, ComparePath> q;
    q.push(Path{pos, dir, 0});

    map<pair<Pos, Pos>, size_t> scores;
    map<pair<Pos, Pos>, set<pair<Pos, Pos>>> backtrack;
    unordered_set<Path> ends;
    size_t bestscore = SIZE_MAX;

    while (!q.empty()) {
        auto item = q.top();
        q.pop();
        if (item.pos == end) {
            if (item.score > bestscore) {
                break;
            }
            if (item.score < bestscore) {
                bestscore = item.score;
            }
            ends.insert(item);

        } else {
            vector<tuple<Pos, Pos, size_t>> steps = {
                {item.pos + item.dir, item.dir, item.score + 1},
                {item.pos, RotateClockwise(item.dir), item.score + 1000},
                {item.pos, RotateCounterClockwise(item.dir), item.score + 1000},
            };
            for (auto &[npos, ndir, nscore] : steps) {
                auto key = make_pair(npos, ndir);
                if (!walls.contains(npos)) {
                    if (!scores.contains(key)) {
                        scores[key] = SIZE_MAX;
                    }
                    if (nscore < scores[key]) {
                        scores[key] = nscore;
                        q.push(Path{npos, ndir, nscore});
                        backtrack[make_pair(npos, ndir)].clear();
                        backtrack[make_pair(npos, ndir)].insert(make_pair(item.pos, item.dir));
                    } else if (nscore == scores[key]) {
                        backtrack[make_pair(npos, ndir)].insert(make_pair(item.pos, item.dir));
                    }
                }
            }
        }
    }


    set<Pos> traversed;
    set<pair<Pos, Pos>> seen;
    vector<pair<Pos, Pos>> s;
    for (auto &end : ends) {
        if (end.score != bestscore)
            continue;
        s.push_back(make_pair(end.pos, end.dir));
        while (s.size()) {
            auto item = s.back();
            s.pop_back();
            if (seen.contains(item)) {
                continue;
            }
            seen.insert(item);
            traversed.insert(item.first); 
            if (backtrack.contains(item)) {
                for (auto &ele : backtrack[item]) {
                    s.push_back(ele);
                }
            }
        }
    }
    return make_pair(bestscore, traversed.size());
}

int main() {
    auto fp = ifstream("../data/day16.txt");
    Pos start;
    Pos end;
    Walls walls;
    for (string line; getline(fp, line);) {

        for (auto col : iota(0ul, line.size())) {
            auto c = line[col];
            Pos p{(int)R, (int)col};
            switch (c) {
                case '#':
                    walls.insert(p);
                    break;
                case 'S':
                    start = p;
                    break;
                case 'E':
                    end = p;
                    break;
            }
        }
        R++;
        C = std::max(C, line.size());
    }
    auto [res1, res2] = Walk(walls, start, DIRECTIONS[1], end);
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
