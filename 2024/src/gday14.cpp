#include <raylib.h>
#include "advent.hpp"

struct Robot {
    Pos pos;
    Pos vel;
};

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

int GetNearCount(vector<Robot> &robots) {
    int nearcount = 0;
    for (auto &robot : robots) {
        auto differences = robots
            | views::filter([&robot] (auto &other){
                return &other != &robot;
            })
            | views::transform([&robot](auto &other) {
                return robot.pos - other.pos;
            });
        bool anynear = ranges::any_of(differences, [](auto diff) {
            return std::abs(diff.first) <= 1 && std::abs(diff.second) <= 1;
        });
        if (anynear)
            nearcount += 1;
    }
    return nearcount;
}

int main() {
    Pos dim = {
        101, 103
    };
    int tileSize = 8;
    const int WIDTH = (int) dim.first * tileSize;
    const int HEIGHT = (int) dim.second * tileSize;
    InitWindow(WIDTH, HEIGHT, "Day 14");
    auto robots = ParseRobots("../data/day14.txt");

    bool animate = true;

    int i = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

        }
        Simulate(robots, dim);
        i++;
        for (auto &robot : robots) {
            DrawRectangle(robot.pos.first * tileSize,
                          robot.pos.second * tileSize,
                          tileSize, tileSize, RED);
        }

        DrawText(TextFormat("I=%d", i), 10, 10, 24, RED);

        WaitTime(0.05);
        EndDrawing();
    }
}
