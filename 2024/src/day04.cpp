#include "advent.hpp"

int SearchWord(vector<string> &grid, string word, int x, int y, int dx, int dy) {

    int xx = x;
    int yy = y;

    auto inRange = [&grid](int _x, int _y) {
        return _x >= 0 && _y >= 0 && _x < grid[0].size() && _y < grid.size();
    };
    for (size_t i = 0; i < word.size(); ++i) {
        if (!inRange(xx, yy) || word[i] != grid[yy][xx]) {
            return 0;
        }
        xx += dx;
        yy += dy;
    }
    return 1;
}

int SearchXMas(vector<string> &grid, int x, int y) {

    int xx = x;
    int yy = y;

    auto inRange = [&grid](int _x, int _y) {
        return _x >= 0 && _y >= 0 && _x + 2 < grid[0].size() && _y + 2 < grid.size();
    };
    if (!inRange(xx, yy)) {
        return 0;
    }
    if (grid[yy][xx] == 'M' || grid[yy][xx] == 'S') {
        string first;
        first += grid[yy + 0][xx + 0];
        first += grid[yy + 1][xx + 1];
        first += grid[yy + 2][xx + 2];
        if (first != "MAS" && first != "SAM") {
            return 0;
        }

        string second;
        second += grid[yy + 0][xx + 2];
        second += grid[yy + 1][xx + 1];
        second += grid[yy + 2][xx + 0];
        if (second != "MAS" && second != "SAM") {
            return 0;
        }
        return 1;
    }
    return 0;
}


int main() {
    auto fp = ifstream("../data/day04.txt");

    int res1 = 0;
    int res2 = 0;

    vector<string> grid;
    for (string line; getline(fp, line);) {
        grid.push_back(line);
    }
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[0].size(); ++x) {

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (SearchWord(grid, "XMAS", x, y, dx, dy)) {
                        res1++;
                    }
                }
            }

            if (SearchXMas(grid, x, y)) {
                res2++;
            }
        }
    }
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
