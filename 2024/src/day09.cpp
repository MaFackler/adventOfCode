#include "advent.hpp"


int GetAmountOfSameValue(auto begin, auto end) {
    auto it = begin;
    while (it != end && *begin == *it) {
        it++;
    }
    return (int) (it - begin);
}

void PrintMemory(const vector<i64> &memory) {
    for (i64 value : memory) {
        assert(value <= 9);
        char c = value == -1 ? '.' : '0' + (int) value;
        cout << c;
    }
    cout << "\n";
}

auto SearchFreeSpace(vector<i64> &memory, int n) {
    auto res = std::find(memory.begin(), memory.end(), -1);
    int freeN = GetAmountOfSameValue(res, memory.end());
    while (res != memory.end() && freeN < n) {
        res = std::find(res + 1, memory.end(), -1);
        freeN = GetAmountOfSameValue(res, memory.end());
    }
    return res;
}

void Solve2(vector<i64> &memory) {
    for (auto valueIt = memory.rbegin(); valueIt != memory.rend();) {
        int valueN = GetAmountOfSameValue(valueIt, memory.rend());
        if (*valueIt == -1) {
            valueIt++;
            continue;
        }

        auto freeSpaceIt = SearchFreeSpace(memory, valueN);
        if (freeSpaceIt != memory.end() && (freeSpaceIt < valueIt.base())) {
            std::fill(freeSpaceIt, freeSpaceIt + valueN, *valueIt);
            std::fill(valueIt, valueIt + valueN, -1);
        }
        valueIt += valueN;
    }
}

void Solve1(vector<i64> &memory) {
    auto freeSpaceIt = std::find(memory.begin(), memory.end(), -1);
    for (auto valueIt = memory.rbegin(); valueIt != memory.rend(); ++valueIt) {
        if (*valueIt == -1) {
            continue;
        }
        if (freeSpaceIt < valueIt.base()) {
            *freeSpaceIt = *valueIt;
            *valueIt = -1;
            freeSpaceIt = std::find(freeSpaceIt, memory.end(), -1);
        }
    }
}

i64 Checksum(const vector<i64> &memory) {
    i64 res = 0;
    for (size_t i = 0; i < memory.size(); ++i) {
        i64 value = memory[i];
        if (value == -1) {
            value = 0;
        }
        res += i * value;
    }
    return res;
}


int main() {
    auto fp = ifstream("../data/day09.txt");

    i64 res1 = 0;
    i64 res2 = 0;
    vector<i64> memory;
    i64 id = 0;
    for (string line; getline(fp, line);) {
        for (size_t i = 0; i < line.size(); ++i) {
            i64 value = line[i] - '0';
            if ((i % 2) == 0) {
                memory.insert(memory.end(), value, id);
                id++;
            } else {
                memory.insert(memory.end(), value, -1);
            }
        }
    }

    vector<i64> memory2 = memory;
    Solve1(memory);
    Solve2(memory2);
    
    res1 = Checksum(memory);
    res2 = Checksum(memory2);

    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
