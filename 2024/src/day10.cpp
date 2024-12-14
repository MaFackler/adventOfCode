#include "advent.hpp"

struct Path {
    Pos pos;
    int value;
};

bool operator==(Path a, Path b) {
    return a.pos.first == b.pos.first && a.pos.second == b.pos.second && a.value == b.value;
}

int Solve2(Grid &grid, Pos start) {
    std::deque<Path> q = {{.pos=start, .value=0}};
    int res = 0;
    for (;!q.empty(); q.pop_front()) {
        auto item = q.front();
        for (auto dir : DIRECTIONS) {
            auto pos = item.pos + dir;
            if (InRange(grid, pos)) {
                int cvalue = grid[pos.second][pos.first] - '0';
                if (cvalue == (item.value + 1)) {
                    if (cvalue == 9) {
                        res++;
                    } else {
                        Path gopath = {.pos=pos, .value=cvalue};
                        q.push_back(gopath);
                    }
                }
            }

        }
    }

    return res;
}

int Solve1(Grid &grid, Pos start) {
    std::deque<Path> q = {{.pos=start, .value=0}};
    set<Pos> ends;
    for (;!q.empty(); q.pop_front()) {
        auto item = q.front();
        for (auto dir : DIRECTIONS) {
            auto pos = item.pos + dir;
            if (InRange(grid, pos)) {
                int cvalue = grid[pos.second][pos.first] - '0';
                if (cvalue == (item.value + 1)) {
                    if (cvalue == 9) {
                        ends.insert(pos);
                    } else {
                        Path gopath = {.pos=pos, .value=cvalue};
                        if (!ranges::contains(q, gopath)) {
                            q.push_back(gopath);
                        }
                    }
                }
            }

        }
    }

    return (int) ends.size();
}

int main() {
    auto fp = ifstream("data/day10.txt");

    int res1 = 0;
    int res2 = 0;
    Grid grid;

    for (string line; getline(fp, line);) {
        grid.push_back(line);
    }

    auto starts = IterGrid(grid)
        | views::filter([](auto tup) {
            auto [y, x, c] = tup;
            return c == '0';
        })
        | views::transform([](auto tup) {
            auto [y, x, c] = tup;
            return Pos{x, y};
        });

    for (auto start : starts) {
        res1 += Solve1(grid, start);
        res2 += Solve2(grid, start);
    }

    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
