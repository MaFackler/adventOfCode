#include "advent.hpp"

using Walls = set<Pos>;

bool InRange(Pos pos, size_t x, size_t y) {
    return 0 <= pos.first && pos.first < x && 0 <= pos.second && pos.second < y;

}
#if 0
struct Path {
    Pos pos;
    Pos dir;
    Pos cheat;
    size_t score;

    bool operator>(const Path &other) const {
        return score > other.score;
    }

    bool operator==(const Path &other) const {
        return pos == other.pos && dir == other.dir && cheat == other.cheat && score == other.score;
    }
};

namespace std {
    template<>
    struct hash<Path> {
        size_t operator()(const Path &p) const {
            return std::hash<Pos>()(p.pos)
                ^ (std::hash<Pos>()(p.dir) << 1)
                ^ (std::hash<Pos>()(p.cheat) << 2)
                ^ (std::hash<size_t>()(p.score) << 3);
        }
    };

}

// STUPID VERSION
vector<size_t> Walk(Walls walls, Pos start, Pos end, size_t X, size_t Y, size_t threshold) {
    std::priority_queue<Path, vector<Path>, std::greater<Path>> q;
    q.push({start, DIRECTIONS[0], Pos{-1, -1}, 0});
    q.push({start, DIRECTIONS[1], Pos{-1, -1}, 0});
    q.push({start, DIRECTIONS[2], Pos{-1, -1}, 0});
    q.push({start, DIRECTIONS[3], Pos{-1, -1}, 0});

    vector<size_t> results;
    unordered_set<Path> seen;

    auto CanCheat = [](Walls &walls, Path &item, Pos dir, size_t x , size_t y) {
        bool res = item.cheat.first == -1 && item.cheat.second == -1;
        res &= InRange(item.pos + dir, x, y);
        res &= InRange(item.pos + dir + dir, x, y);
        res &= walls.contains(item.pos + dir);
        res &= !walls.contains(item.pos + dir + dir);
        return res;
    };
    vector<size_t> scores;
    set<Pos> cheats;
    while (!q.empty()) {
        auto item = q.top();
        q.pop();

        if (seen.contains(item)) {
            continue;
        }
        seen.insert(item);

        if (item.pos == end) {
            // println("GOT {} {} score={}", item.pos.first, item.pos.second, item.score);
            if (threshold == 0) {
                scores.push_back(item.score);
                break;
            } else {
                if (item.score <= threshold) {
                    scores.push_back(item.score);
                }
                if (item.score > threshold) {
                    break;
                }
            }
        }

        for (auto &dir : DIRECTIONS) {
            if (item.dir == Pos{-dir.first, -dir.second}) {
                continue;
            }
            if (InRange(item.pos + dir, X, Y)) {
                if (!walls.contains(item.pos + dir)) {
                    q.push({item.pos + dir, dir, item.cheat, item.score + 1});
                } else if (threshold > 0 && CanCheat(walls, item, dir, X, Y)) {
                    cheats.insert(item.pos + dir);
                    q.push({item.pos + dir + dir, dir, item.pos + dir, item.score + 2});
                }
            }
        }
    }
    return scores;
}
#endif

int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("../data/day20.txt");
    Walls walls;
    size_t X = 0;
    size_t Y = 0;
    Pos start;
    Pos end;
    for (string line; getline(fp, line);) {
        for (size_t x = 0; x < line.size(); ++x) {
            char c = line[x];
            Pos p{(int)x, (int)Y};
            if (c == '#') {
                walls.insert(p);
            } else if (c == 'E') {
                end = p;
            } else if (c == 'S') {
                start = p;
            }

        }
        Y++;
        X = std::max(X, line.size());
    }


    deque<pair<Pos, int>> q;
    vector<vector<int>> costs(Y, vector<int>(X, -1));

    q.push_back(make_pair(start, 0));
    while(!q.empty()) {
        auto [pos, cost] = q.front(); 
        q.pop_front();
        costs[pos.second][pos.first] = cost;
        if (pos == end) {
            // println("{} {}", pos.first, pos.second);
            break;
        }
        for (auto &dir : DIRECTIONS) {
            auto npos = pos + dir;
            if (InRange(npos, X, Y) && !walls.contains(npos) && costs[npos.second][npos.first] == -1) {
                q.push_back(make_pair(npos, cost + 1));
            }
        }
    }

    auto FindCheats = [&walls, &X, &Y, &costs](int value, int offset) {

        int res = 0;
        for (int y = 0; y < Y; ++y) {
            for (int x = 0; x < X; ++x) {
                for (int radius = 2; radius <= value; ++radius) {
                    for (int dy = 0; dy <= radius; ++dy) {
                        int dx = radius - dy;
                        set<Pos> positions;
                        positions.insert({x + dx, y + dy});
                        positions.insert({x - dx, y + dy});
                        positions.insert({x + dx, y - dy});
                        positions.insert({x - dx, y - dy});
                        for (Pos other : positions) {
                            if (!InRange(other, X, Y)) continue;
                            if (walls.contains(other)) continue;
                            if ((costs[y][x] - costs[other.second][other.first]) >= (offset + radius)) {
                                res++;
                            }
                        }
                        
                    }
                }
            }
        }
        return res;
    };
    
    res1 = FindCheats(2, 100);
    res2 = FindCheats(20, 100);
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
