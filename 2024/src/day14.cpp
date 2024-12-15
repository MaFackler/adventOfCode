#include "advent.hpp"

struct Robot {
    Pos pos;
    Pos vel;
};

void Simulate(vector<Robot> &robots, Pos dim) {
    for (auto &robot : robots) {
        Pos pos = robot.pos + robot.vel;
        pos.first = pos.first % dim.first;
        pos.second = pos.second % dim.second;
        if (pos.first < 0) {
            pos.first += dim.first;
        }
        if (pos.second < 0) {
            pos.second += dim.second;
        }
        assert(pos.first >= 0);
        assert(pos.first < dim.first);
        assert(pos.second >= 0);
        assert(pos.second < dim.second);
        robot.pos = pos;
    }
}

vector<Robot> ParseRobots(const char *filename) {
    auto fp = ifstream(filename);
    vector<Robot> robots;
    for (string line; getline(fp, line);) {
        int px, py, vx, vy;
        sscanf_s(line.c_str(), "p=%d,%d v=%d,%d", &px, &py, &vx, &vy);
        robots.push_back({.pos={px, py}, .vel={vx, vy}});
    }
    return robots;
}

u64 Solve1(vector<Robot> &robots, Pos dim) {
    std::array<size_t, 4> counts = {0};
    Pos quadrant = {
        (dim.first - 1) / 2,
        (dim.second - 1) / 2,
    };
    for (auto &robot : robots) {
        bool onquad = robot.pos.first == (dim.first / 2);
        onquad |= robot.pos.second == (dim.second / 2);
        if (onquad) {
            continue;
        }
        int x = robot.pos.first / (quadrant.first + 1);
        int y = robot.pos.second / (quadrant.second + 1);
        assert(x >= 0);
        assert(x < 2);
        assert(y >= 0);
        assert(y < 2);
        counts[y * 2 + x]++;
    }
    u64 res = ranges::fold_left(counts, 1, std::multiplies{});
    return res;

}

void PrintRobots(vector<Robot> robots, Pos dim) {
    for (int y = 0; y < dim.second; ++y) {
        for (int x = 0; x < dim.first; ++x) {
            char c = '.';
            size_t count = ranges::count_if(robots, [&x, &y](auto &robot) {
                return robot.pos == Pos{x, y};
            });
            if (count) {
                c = '0' + (int) count;
            }
            cout << c;
        }
        cout << "\n";
    }
}

int SearchComponents(vector<Robot> &robots, Pos dim) {
    set<Pos> seen;
    std::deque<Pos> q;
    auto positions = robots
        | views::transform([](auto ele) { return ele.pos; })
        | ranges::to<vector>();
    // ranges::copy(q, std::back_inserter(positions));
    for (auto p : positions) {
        q.push_back(p);
    }

    assert(!q.empty());
    int res = 0;
    int components = 1;
    for (;!q.empty(); q.pop_front()) {
        auto pos = q.front();
        if (seen.contains(pos)) {
            continue;
        }
        seen.insert(pos);
        for (auto dir : DIRECTIONS) {
            Pos pp = pos + dir;
            if (ranges::contains(positions, pp)) {
                q.push_front(pp);
                components++;
            } else {
                res = std::max(res, components);
                components = 1;
            }
        }
    }
    return res;
}

int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto robots = ParseRobots("../data/day14.txt");
    auto robots2 = robots;
    auto robots3 = robots;

    Pos dim = {101, 103};


    for (auto _ : views::iota(0, 100)) {
        Simulate(robots, dim);
    }


    res1 = Solve1(robots, dim);

    vector<std::pair<int, u64>> results;
    int maxcomp = 0;
    int maxi = 0;
    for (auto i : views::iota(0, 10000)) {
        Simulate(robots2, dim);
        // auto res = Solve1(robots2, dim);
        auto components = SearchComponents(robots2, dim);
        // println("COmp i={} comp={}", i, components);
        if (components > maxcomp) {
            maxi = i;
        }
        maxcomp = std::max(maxcomp, components);

    }
    res2 = maxi + 1;
#if 0
    for (auto i : views::iota(0ul, res2)) {
        Simulate(robots3, dim);
    }
    PrintRobots(robots3, dim);
#endif
    println("Solution1: {}", res1);
    println("Solution2: {}", maxi + 1);
    return 0;
}
