#include "advent.hpp"


enum struct Direction : int {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
};
using Pos = std::tuple<int, int>;
using Walls = vector<Pos>;
using Visited = map<Pos, unordered_set<Direction>>;

Direction rotate(Direction dir) {
    int value = (int) dir;
    value = ++value % 4;
    return (Direction) value;
}

bool Walk(Walls &walls, Pos &pos, Direction &dir, int maxX, int maxY) {
    int x = std::get<0>(pos);
    int y = std::get<1>(pos);
    switch (dir) {
        case Direction::NORTH:
            y--;
            break;
        case Direction::EAST:
            x++;
            break;
        case Direction::SOUTH:
            y++;
            break;
        case Direction::WEST:
            x--;
            break;
    }

    Pos newPos = {x, y};
    if (std::find(walls.begin(), walls.end(), newPos) != walls.end()) {
        dir = rotate(dir);
        Walk(walls, pos, dir, maxX, maxY);
    } else {
        pos = newPos;
    }
    return x >= 0 && y >= 0 && x <= maxX && y <= maxY;
};

bool CheckRight(Visited visited, vector<Pos> &walls, Pos pos, Direction dir, int maxX, int maxY) {
    dir = rotate(dir);
    Direction startdir = dir;
    Pos startpos = pos;
    Visited visitedWalk;
    visitedWalk[pos].insert(dir);
    while (Walk(walls, pos, dir, maxX, maxY)) {
        if (visitedWalk.contains(pos) && visitedWalk[pos].contains(dir)) {
            return true;
        }
        visitedWalk[pos].insert(dir);
        if (visited.contains(pos) && visited[pos].contains(dir)) {
            return true;
        }
    }
    return false;
}

int Solve1(Walls walls, Pos start, Direction dir, int maxX, int maxY) {
    Pos guard = start;
    Visited visited;
    visited[start].insert(dir);
    map<Pos, bool> obstacles;
    while (Walk(walls, guard, dir, maxX, maxY)) {
        visited[guard].insert(dir);
    }
    return (int) visited.size();
}

bool SearchLoop(Walls walls, Pos start, Direction dir, int maxX, int maxY, int x, int y) {
    Visited currentVisited = {};
    Pos pos = start;
    currentVisited[pos].insert(dir);
    vector<Pos> currentWalls = walls;
    if (std::find(currentWalls.begin(), currentWalls.end(), (Pos){x, y}) == currentWalls.end()) {
        currentWalls.push_back({x, y});
        while (Walk(currentWalls, pos, dir, maxX, maxY)) {
            if (currentVisited.contains(pos) && currentVisited[pos].contains(dir)) {
                return true;
                break;
            }
            currentVisited[pos].insert(dir);
        }
    }
    return false;
}

int Solve2(Walls walls, Pos start, Direction dir, int maxX, int maxY) {
    Pos guard = start;
    int res = 0;
    for (int y = 0; y <= maxY; ++y) {
        for (int x = 0; x <= maxX; ++x) {
            bool found = SearchLoop(walls, start, dir, maxX, maxY, x, y);
            res += found;
        }
    }
    return res;
}


int main() {
    auto fp = ifstream("data/day06.txt");

    int res1 = 0;
    int res2 = 0;
    int y = 0;
    Pos start{};
    Walls walls;

    Direction dir = Direction::NORTH;

    int maxX = 0;
    int maxY = 0;
    for (string line; getline(fp, line);) {
        for (int x = 0; x < line.size(); ++x) {
            char c = line[x];
            Pos pos{x, y};
            if (c == '^') {
                start = pos;
            } else if (c == '#') {
                walls.push_back(pos);
            }
            maxX = std::max(x, maxX);
        }
        y += 1;
    }
    
    maxY = y - 1;
    res1 = Solve1(walls, start, dir, maxX, maxY);
    res2 = Solve2(walls, start, dir, maxX, maxY);

    print() << "Solution1: " << res1;
    print() << "Solution2: " << res2;
    return 0;
}
