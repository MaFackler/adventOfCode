#include "advent.hpp"

enum struct Movement : char {
    LEFT = '<',
    RIGHT = '>',
    DOWN = 'v',
    UP = '^',
};

static std::map<Movement, Pos> DIRS = {
    {Movement::LEFT, Pos{-1, 0}},
    {Movement::RIGHT, Pos{1, 0}},
    {Movement::DOWN, Pos{0, 1}},
    {Movement::UP, Pos{0, -1}},
};

enum struct Entity : char {
    WALL = '#',
    BOX = 'O',
    ROBOT = '@',
};

struct Box {
    Pos pos;
    Pos size;

    bool operator==(const Box &other) const {
        return pos == other.pos && size == other.size;
    }

};

using Movements = vector<Movement>;
using Walls = unordered_set<Box>;
using Boxes = vector<Box>;

namespace std {
    template<>
    struct hash<Box> {
        std::size_t operator()(const Box &p) const {
            return std::hash<Pos>()(p.pos) ^ (std::hash<Pos>()(p.size) << 1);
        }
    };
}


bool BoxCollide(Box a, Box b) {
    return a.pos.first < b.pos.first + b.size.first &&
           a.pos.first + a.size.first > b.pos.first &&
           a.pos.second < b.pos.second + b.size.second &&
           a.pos.second + a.size.second > b.pos.second;
}

bool WallsCollideWithBox(Walls &walls, Box box) {
    return ranges::any_of(walls, [&box](auto &wall) { return BoxCollide(wall, box); });
}

std::pair<bool, vector<Box*>> Move(Walls &walls, Boxes &boxes, Box robot, Movement direction) {
    deque<Box> q = {robot};
    unordered_set<Box> seen;
    vector<Box*> res;
    for (;!q.empty(); q.pop_front()) {
        Box item = q.front();
        if (seen.contains(item)) {
            continue;
        }
        seen.insert(item);
        item.pos = item.pos + DIRS[direction];
        if (WallsCollideWithBox(walls, item)) {
            res.clear();
            return std::make_pair(false, res);
        } else {
            for (Box &box : boxes) {
                auto collides = BoxCollide(box, item);
                if (collides) {
                    q.push_back(box);
                    if (!ranges::contains(res, &box)) {
                        res.push_back(&box);
                    }
                }
            }
        }
    }
    return std::make_pair(true, res);
}

Box RobotWalk(Walls &walls, Boxes &boxes, Box robot, Movement direction) {
    // auto boxit = boxes.find(newpos);
    auto [canmove, objects] = Move(walls, boxes, robot, direction);
    if (canmove) {
        for (Box *box : objects) {
            box->pos = box->pos + DIRS[direction];
        }
        robot.pos = robot.pos + DIRS[direction];
    }
    return robot;
}


void PrintGrid(Walls &walls, Boxes &boxes, Box robot, size_t X, size_t Y) {
    for (size_t y : views::iota(0ul, Y)) {
        for (size_t x : views::iota(0ul, X)) {
            Box cell{{(int) x, (int) y}, {1, 1}};
            char c = '.';
            if (WallsCollideWithBox(walls, cell)) {
                c = (char) Entity::WALL;
            } else if (cell.pos == robot.pos) {
                c = (char) Entity::ROBOT;
            } else {
                for (auto box : boxes) {
                    auto collision = BoxCollide(box, cell);
                    if (collision) {
                        c = 'O';
                    }
                }
            }
            std::print("{}", c);
        }
        println("");
    }
}

int main() {

    auto fp = ifstream("../data/day15.txt");
    u64 res1 = 0;
    u64 res2 = 0;
    bool first = true;
    Movements movements;

    Box robot;
    Walls walls;
    Boxes boxes;

    Box robot2;
    Walls walls2;
    Boxes boxes2;

    assert (BoxCollide(Box{{0, 0}, {1, 1}}, Box{{0, 0}, {1, 1}}));
    assert (!BoxCollide(Box{{0, 0}, {1, 1}}, Box{{1, 0}, {1, 1}}));
    assert (BoxCollide(Box{{0, 0}, {1, 1}}, Box{{0, 0}, {2, 1}}));
    assert (BoxCollide(Box{{1, 0}, {1, 1}}, Box{{0, 0}, {2, 1}}));
    assert (!BoxCollide(Box{{2, 0}, {1, 1}}, Box{{0, 0}, {2, 1}}));
    assert (!BoxCollide(Box{{0, 1}, {1, 1}}, Box{{0, 0}, {2, 1}}));
    assert (!BoxCollide(Box{{1, 1}, {1, 1}}, Box{{0, 0}, {2, 1}}));

    size_t Y = 0;
    size_t X = 0;
    for (string line; getline(fp, line);) {
    if (line == "") {
        first = false;
    }
    if (first) {
        for (int x = 0; x < line.size(); ++x) {
            char c = line[x];
            Pos p{(int) x, (int) Y};
            Pos p2{(int) x * 2, (int) Y};
            switch ((Entity) c) {
                case Entity::WALL: {
                    walls.insert(Box{p, {1, 1}});
                    walls2.insert(Box{p2, {2, 1}});
                } break;
                case Entity::BOX: {
                    boxes.push_back(Box{p, {1, 1}});
                    boxes2.push_back(Box{p2, {2, 1}});
                } break;
                case Entity::ROBOT: {
                    robot = Box{p, {1, 1}};
                    robot2 = Box{p2, {1, 1}};
                } break;
            }
        }
        Y++;
        X = std::max(X, line.size());
    } else {
        for (char c : line) {
            movements.push_back((Movement) c);
        }
    }
    }

    for (Movement movement : movements) {
        robot = RobotWalk(walls, boxes, robot, movement);
    }

    auto gps = [](auto &boxes) -> u64 {
        auto togps = boxes
            | views::transform([](auto &&box) { return 100 * box.pos.second + box.pos.first; });
        return ranges::fold_left(togps, 0, std::plus{});
    };

    for (Movement movement : movements) {
        robot2 = RobotWalk(walls2, boxes2, robot2, movement);
    }

    res1 = gps(boxes);
    res2 = gps(boxes2);
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
