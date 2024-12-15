#include "advent.hpp"

using Pos = std::pair<int, int>;
using Grid = vector<string>;
using Visited = set<Pos>;


int Search1(Grid &grid, Visited &visited, Pos start, char c) {
    set<Pos> region = {start};
    std::deque<Pos> q = {start};
    visited.insert(start);
    vector<Pos> fences;
    map<Pos, set<Pos>> sides;
    for (;!q.empty(); q.pop_front()) {
        auto p = q.front();
        visited.insert(p);
        for (auto dir : DIRECTIONS) {
            Pos dp{p.first + dir.first, p.second + dir.second};
            if (InRange(grid, dp)) {
                char other = grid[dp.second][dp.first];
                if (other == c) {
                    region.insert(dp);
                    if (!visited.contains(dp) && !ranges::contains(q, dp)) {
                        visited.insert(dp);
                        q.push_back(dp);
                    }
                } else {
                    fences.push_back(dp);
                    sides[dir].insert(dp);
                }
            } else {
                fences.push_back(dp);
                sides[dir].insert(dp);
            }

        }
    }
    return (int) region.size() * (int) fences.size();
}


int Search2(Grid &grid, Visited &visited, Pos start, char c) {
    set<Pos> region = {start};
    std::deque<Pos> q = {start};
    visited.insert(start);
    vector<Pos> fences;
    map<Pos, set<Pos>> sides;
    while (!q.empty()) {
        auto p = q.front();
        q.pop_front();
        visited.insert(p);
        for (auto dir : DIRECTIONS) {
            Pos dp{p.first + dir.first, p.second + dir.second};
            if (InRange(grid, dp)) {
                char other = grid[dp.second][dp.first];
                if (other == c) {
                    region.insert(dp);
                    if (!visited.contains(dp) && !ranges::contains(q, dp)) {
                        visited.insert(dp);
                        q.push_back(dp);
                    }
                } else {
                    fences.push_back(dp);
                    sides[dir].insert(dp);
                }
            } else {
                fences.push_back(dp);
                sides[dir].insert(dp);
            }

        }
    }

    int nsides = 0;
    for (auto &[_, positions] : sides) {
        set<Pos> seen;
        for (auto &pos : positions) {
            if (seen.contains(pos)) {
                continue;
            }
            nsides++;
            std::deque<Pos> sideQ = {pos};
            for (;!sideQ.empty(); sideQ.pop_front()) {
                auto &item = sideQ.front();
                if (seen.contains(item)) {
                    continue;
                }
                seen.insert(item);
                for (auto &dir : DIRECTIONS) {
                    Pos p = {item.first + dir.first, item.second + dir.second};
                    if (ranges::contains(positions, p)) {
                        sideQ.push_back(p);
                    }
                }
            }
        }

    }

    //cout << c << ": " << region.size() << " * " << fences.size() << "\n";
    // cout << c << ": " << region.size() << " * " << nsides << "\n";

    return (int) region.size() * nsides;

}

int Solve1(Grid &grid) {
    Visited visited;
    int res = 0;
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[0].size(); ++x) {
            char c = grid[y][x];
            if (!visited.contains(Pos{x, y})) {
                res += Search1(grid, visited, {x, y}, c);
            }
        }
    }
    return res;
}

int Solve2(Grid &grid) {
    Visited visited;
    int res = 0;
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[0].size(); ++x) {
            char c = grid[y][x];
            if (!ranges::contains(visited, Pos{x, y})) {
                res += Search2(grid, visited, {x, y}, c);
            }
        }
    }
    return res;
}


int main() {
    auto fp = ifstream("../data/day12.txt");

    Grid grid;
    u64 res1 = 0;
    u64 res2 = 0;
    for (string line; getline(fp, line);) {
        grid.push_back(line);
    }
    res1 = Solve1(grid);
    res2 = Solve2(grid);
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
