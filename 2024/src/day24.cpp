#include "advent.hpp"


struct Gate {
    string first;
    string second;
    string operant;
    string result;
};



auto starts_with_filter(string &&letter) {
    auto res = views::filter([&letter](auto &&pair) { return pair.first.starts_with(letter); });
    return res;
}

u64 toU64(map<string, bool> values, string &&letter) {
    auto order = values
        | starts_with_filter(std::move(letter))
        | views::transform([](auto &&pair) { return pair.first; })
        | ranges::to<vector>();
    ranges::sort(order, std::greater<>());
    u64 res = 0;
    for (auto &key : order) {
        auto value = values[key];
        res += value;
        res = res << 1;
    }
    res = res >> 1;
    return res;
}

string GetRegFromI(size_t i, string letter) {
    std::ostringstream ss;
    ss << letter;
    if (i < 10) {
        ss << "0";
        ss << i;
    } else {
        ss << i;
    }
    return ss.str();
}

void Graphviz(map<string, Gate> &gates) {
    auto fp = std::ofstream("./graph.viz");
    fp << "digraph G {\n";
    for (auto &[key, value] : gates) {
        fp << "  " << value.first << " -> " << key << "[label=\"" << value.operant << "\"]\n";
        fp << "  " << value.second << " -> " << key << "[label=\"" << value.operant << "\"]\n";
    }
    fp << "}\n";

}

u64 Calculate(map<string, bool> &values, map<string, Gate> gates) {
    deque<Gate> q;
    for (auto &[key, value] : gates) {
        q.push_back(value);
    }
    while (!q.empty()) {
        auto gate = q.front();
        q.pop_front();
        if (!values.contains(gate.first) || !values.contains(gate.second)) {
            // println("skip gate {} {}", gate.first, gate.second);
            q.push_back(gate);
            continue;
        }
        if (gate.operant == "AND") {
            values[gate.result] = values[gate.first] && values[gate.second];
        } else if (gate.operant == "OR") {
            values[gate.result] = values[gate.first] || values[gate.second];
        } else if (gate.operant == "XOR") {
            values[gate.result] = values[gate.first] != values[gate.second];
        } else {
            assert(false);
        }
    }
    u64 res = toU64(values, "z");
    return res;
}

vector<Gate> GetGates(map<string, Gate> gates, string first, string second, string operant) {
    vector<Gate> res;
    for (auto &[key, value] : gates) {
        if ((value.first == first && value.second == second) ||
            (value.first == second && value.second == first)) {
            if (value.operant == operant) {
                res.push_back(value);
            }
        }
    }
    return res;
}


void Solve2(map<string, bool> values, map<string, Gate> gates, size_t bits, u64 x, u64 y) {


    auto Swap = [&gates](string first, string second) {
        auto tmp = gates[first];
        assert(tmp.result == first);
        tmp.result = second;
        gates[first] = gates[second];
        gates[first].result = first;
        gates[second] = tmp;
    };

    Swap("thm", "z08");
    Swap("wss", "wrm");
    Swap("hwq", "z22");
    Swap("gbs", "z29");
    // Graphviz(gates);

    auto expectedOrginal = x + y;

    for (size_t i = 0; i < bits; ++i) {
        u64 mask = (0x01ull << (i + 1)) - 1;
        u64 xx = (x & mask);
        u64 yy = (y & mask);
        auto v = values;

        // wrm, wss
        for (size_t b = 0; b < bits; ++b) {
            auto regx = GetRegFromI(b, "x");
            auto regy = GetRegFromI(b, "y");
            auto regz = GetRegFromI(b, "z");
            v[regx] = 0;
            v[regy] = 0;
            // v[regz] = 0;
        }
        for (size_t j = 0; j <= i; ++j) {
            auto regx = GetRegFromI(j, "x");
            auto regy = GetRegFromI(j, "y");
            auto bitmask = (1ull << j);
            auto valuex = (xx & bitmask) == bitmask;
            auto valuey = (yy & bitmask) == bitmask;
            // println("Set {} to {}", regx, valuex);
            // println("Set {} to {}\n", regy, valuey);
            v[regx] = valuex;
            v[regy] = valuey;
        }
        auto got = Calculate(v, gates);
        auto expected = xx + yy;
        bool equal = expected == got;

        auto regx = GetRegFromI(i, "x");
        auto regy = GetRegFromI(i, "y");
        auto regz = GetRegFromI(i, "z");
        auto xor1 = GetGates(gates, regx, regy, "XOR");
        assert(xor1.size() == 1);
        auto and1 = GetGates(gates, regx, regy, "AND");
        assert(and1.size() == 1);
        if (!equal) {
            // println("\t{:b}\n\t{:b}\ng=\t{:b}\ne=\t{:b}", xx, yy, got, expected);



#if 0
            assert(v["y31"] == false);
            assert(v["x31"] == false);
            assert(v["hvf"] == false);
            assert(v["pfk"] == true);
            assert(v["z29"] == true);
            assert(v["z30"] == true);
            assert(v["z31"] == true);
#endif
            break;
        } else {
            // println("{}\n{}", got, expected);
        }
    }
    // auto got = Calculate(values, gates);
    // println("{:b} {}", expectedOrginal, expectedOrginal);
    // println("{:b} {}", got, got);
}


#if 0
11011101101101111101111010101111
   11100011010100100010000011111
#endif

int main() {

    u64 res1 = 0;
    u64 res2 = 0;
    auto fp = ifstream("../data/day24.txt");
    bool first = true;

    map<string, bool> values;
    map<string, Gate> gates;
    for (string line; getline(fp, line);) {
        istringstream stream(line);
        if (line == "") {
            first = false;
            continue;
        }
        if (first) {
            string reg;
            getline(stream, reg, ':');
            stream.get();
            bool value;
            stream >> value;
            values[reg] = value;
        } else {
            string first, second, operant, dummy, last;
            stream >> first;
            stream >> operant;
            stream >> second;
            stream >> dummy;
            stream >> last;
            gates[last] = Gate(first, second, operant, last);
        }
    }

    size_t bits = (values | starts_with_filter("x") | ranges::to<map>()).size();


    u64 x = toU64(values, "x");
    u64 y = toU64(values, "y");
    {
        auto v = values;
        res1 = Calculate(v, gates);
    }
    x = (0x01ull << 45) - 1; 
    y = x;
    Solve2(values, gates, bits, x, y);
    vector<string> swapping = {
        "thm", "z08",
        "wss", "wrm",
        "hwq", "z22",
        "gbs", "z29",
    };
    ranges::sort(swapping);

    std::ostringstream ss;
    for (size_t i = 0; i < swapping.size(); ++i) {
        ss << swapping[i];
        if (i < (swapping.size() - 1)) {
            ss << ",";
        }
    }

    println("Solution1: {}", res1);
    println("Solution2: {}", ss.str());
    return 0;
}
