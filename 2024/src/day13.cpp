#include "advent.hpp"
#include <queue>


std::pair<int, int> Solve1(int ax, int ay, int bx, int by, int xres, int yres) {

    for (int a = 0; a < 100; ++a) {
        for (int b = 0; b < 100; ++b) {
            int ix = a * ax + b * bx;
            int iy = a * ay + b * by;

            if (ix == xres && iy == yres) {
                return std::make_pair(a, b);
            }
        }

    }

    return std::make_pair(-1, -1);
}

std::pair<i64, i64> Solve2(double ax, double ay, double bx, double by, double xres, double yres) {
    double diff = 10000000000000;
    xres += diff;
    yres += diff;
    double b = (xres * ay - yres * ax) / (bx * ay - by * ax);
    double rest = xres - (b * bx);
    double a = rest / ax;

    if (std::modf(b, &b) == 0.0f && std::modf(a, &a) == 0.0f) {
        return std::make_pair((i64) a, (i64)b);
    }
    return std::make_pair(-1, -1);
}


int main() {
    auto fp = ifstream("data/day13.txt");

    i64 res1 = 0;
    i64 res2 = 0;
    for (string line; getline(fp, line);) {
        char c;
        int ax, ay;
        sscanf(line.c_str(), "Button %c: X+%d, Y+%d", &c, &ax, &ay);
        getline(fp, line);
        int bx, by;
        sscanf(line.c_str(), "Button %c: X+%d, Y+%d", &c, &bx, &by);
        getline(fp, line);
        int xres, yres;
        sscanf(line.c_str(), "Prize: X=%d, Y=%d", &xres, &yres);

        getline(fp, line);
        assert(line == "");

        {
            auto [a, b] = Solve1(ax, ay, bx, by, xres, yres);
            if (a >= 0 && b >= 0) {
                res1 += a * 3 + b;
            }
        }
        {
            auto [a, b] = Solve2(ax, ay, bx, by, xres, yres);
            if (a >= 0 && b >= 0) {
                res2 += a * 3 + b;
            }
        }
        
    }
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
