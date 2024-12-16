#include "advent.hpp"


int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("data/day01.e");
    for (string line; getline(fp, line);) {
        println("{}", line);
    }
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
