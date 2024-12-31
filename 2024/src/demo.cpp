#include "advent.hpp"

int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("../data/data25.txt");
    string line;
    while (getline(fp, line)) {
        println("{}", line);
    }

    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
