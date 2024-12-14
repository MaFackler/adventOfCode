#include "advent.hpp"

using Antenna = char;

bool inRange(Pos p, int maxX, int maxY) {
    auto [x, y] = p;
    return x >= 0 && x < maxX && y >= 0 && y < maxY;
}

void SpawnAntinodes(vector<Pos> &res, Pos start, Pos diff, int maxX, int maxY, bool part1) {
    auto n1 = start + diff;
    while (inRange(n1, maxX, maxY)) {
        if (!ranges::contains(res, n1)) {
            res.push_back(n1);
            if (part1) {
                return;
            }
        }
        n1 = n1 + diff;
    }
}

int main() {
    auto fp = ifstream("data/day08.txt");

    int res1 = 0;
    int res2 = 0;
    map<Antenna, vector<Pos>> antennas;
    vector<Pos> antinodes;
    vector<Pos> antinodes2;
    int y = 0;
    int maxX = 0;
    int maxY = 0;
    for (string line; getline(fp, line);) {
        for (int x = 0; x < line.size(); ++x) {
            Antenna c = line[x];
            if (c == '.') {
                continue;
            }
            Pos p = {x, maxY};
            antennas[c].push_back((Pos) {x, maxY});
            antinodes2.push_back(p);
        }
        maxX = std::max(maxX, (int) line.size());
        maxY += 1;
    }


    for (auto &[key, positions] : antennas) {
        for (int i = 0; i < positions.size(); ++i) {
            auto p1 = positions[i];
            for (int j = i; j < positions.size(); ++j) {
                if (j == i) {
                    continue;
                }
                auto p2 = positions[j];
                Pos diff = p2 - p1;
                Pos diffnegative = p1 - p2;
                SpawnAntinodes(antinodes, p2, diff, maxX, maxY, true);
                SpawnAntinodes(antinodes, p1, diffnegative, maxX, maxY, true);

                int sizeBefore = antinodes2.size();
                SpawnAntinodes(antinodes2, p2, diff, maxX, maxY, false);
                SpawnAntinodes(antinodes2, p1, diffnegative, maxX, maxY, false);
            }
        }
    }
#if 0
    for (int y = 0; y < maxY; ++y) {
        for (int x = 0; x < maxX; ++x) {
            char c = '.';
            Pos p{x, y};
            if (ranges::contains(antinodes2, p)) {
                c = '#';
            }
            for (auto &[key, positions] : antennas) {
                if (ranges::contains(positions, p)) {
                    if (c == '#') {
                        c = '^';
                    } else {
                        c = key;
                    }
                    break;
                }
            }
            cout << c;
        }
        cout << "\n";
    }
#endif

    res1 = (int) antinodes.size();
    res2 += (int) antinodes2.size();
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
