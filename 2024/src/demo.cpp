#include "advent.hpp"


int main() {
    auto fp = ifstream("data/day03.txt");

    int res1 = 0;
    int res2 = 0;
    bool enabled = true;
    for (string line; getline(fp, line);) {
        istringstream stream(line);
    }
    print() << "Solution1: " << res1;
    print() << "Solution2: " << res2;
    return 0;
}
