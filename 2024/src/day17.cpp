#include "advent.hpp"


using Instruction = int;
using Opcode = int;


vector<int> RunProgram(vector<int> program, u64 rega, u64 regb, u64 regc) {
    vector<int> output;
    for (int i = 0; i < program.size(); i += 2) {
begin_for:
        auto opcode = program[i];
        auto operant = program[i + 1];

        u64 combo = operant;
        if (combo == 4) {
            combo = rega;
        } else if (operant == 5) {
            combo = regb;
        } else if (operant == 6) {
            combo = regc;
        } else if (operant == 7) {
            // assert(!"INVADID");
        }
        // println("a={} b={} c={}", rega, regb, regc);
        // println("isp={} opcode={} operant={} combo={}", i, opcode, operant, combo);
        if (opcode == 0) {
            // adv division
            // TODO: truncate maybe use a shift operator?
            double tmp = (double) rega / (double) pow(2, combo);
            rega = (u64) tmp;
            // println("adv {}", rega);
        } else if (opcode == 1) {
            // bxl bitwise or
            regb = regb ^ operant;
        } else if (opcode == 2) {
            // bst modulo 8
            regb = combo % 8;
        } else if (opcode == 3) {
            // jnz 
            if (rega != 0 && i != operant) {
                i = operant;
                goto begin_for;
            }
        } else if (opcode == 4) {
            // bxc bitwise xor regb and regc
            regb = regb ^ regc;
        } else if (opcode == 5) {
            // out
            // println("GOT PRINT WITH a={} b={}", rega, regb);
            int value = combo % 8;
            output.push_back(value);
        } else if (opcode == 6) {
            // bdv
            double tmp = (double) rega / (double) pow(2, combo);
            regb = (u64) tmp;
        } else if (opcode == 7) {
            // cdv
            double tmp = (double) rega / (double) pow(2, combo);
            regc = (u64) tmp;
        } else {
            assert(!"INVALID");
        }
    }
    return output;

}

string StringifyOutput(vector<int> output) {
    std::stringstream ss;
    for (int i = 0; i < output.size(); ++i) {
        ss << output[i];
        if (i < output.size() - 1) {
            ss << ",";
        }
    }
    return ss.str();
}

std::tuple<int, int, int, int> RunSequence(int a, int b, int c) {
    b = a % 8;  // 2, 4
    b = b ^ 1;  // 1, 1
    c = a / (1 << b); // # 7, 5
    b = b ^ c;  // 4, 4
    b = b ^ 4;  // 1, 4
    a = a / (1 << 3); // # 0, 3
    return std::make_tuple((b % 8), a, b, c);
}

std::optional<u64> FindA(vector<int> program, int index, size_t a, size_t b, size_t c) {
    if (index < 0) {
        return a;
    } else {
        for (int i = 0; i < 8; ++i) {
            auto number = RunProgram(program, a * 8 + i, b, c)[0];
            if (number == program[index]) {
                // println("{} {} {} {}", string((int) program.size() - index, '.'), a, i, number);
                auto res = FindA(program, index - 1, (a * 8) + i, 0, 0);
                if (res) {
                    return res;
                }
            }
        }
    }
    return std::nullopt;
}

int main() {

    string res1;
    u64 res2 = 0;
    auto fp = ifstream("../data/day17.txt");
    assert(fp.is_open());

    vector<int> program;
    i32 rega, regb, regc;
    for (string line; getline(fp, line);) {
        sscanf(line.c_str(), "Register A: %d", &rega);
        getline(fp, line);
        sscanf(line.c_str(), "Register B: %d", &regb);
        getline(fp, line);
        sscanf(line.c_str(), "Register C: %d", &regc);
        getline(fp, line);
        assert(line == "");
        getline(fp, line);

        line = line.substr(strlen("Program: "), line.size());

        program = views::split(line, ',')
            | views::transform([](auto &&ele) { return std::stoi(string(ele.begin(), ele.end())); })
            | ranges::to<vector>();
    }
    assert(program.size() > 0 && program.size() % 2 == 0);


#if 0
    vector<int> prog = {
        4, 0
    };
    rega = 0;
    regb = 2024;
    regc = 43690;
#endif
#if 0
    out = []
    while (a != 0):
        b = a % 8  # 2, 4
        b = b ^ 1  # 1, 1
        c = a // (2 << (b - 1))  # 7, 5
        b = b ^ c  # 4, 4
        b = b ^ 4  # 1, 4
        a = a // (2 << (3 - 1)) # 0, 3
        out.append(str(b % 8)) # 5, 5
#endif
    // while(A != 0) goto instruction 0

    
    // StringifyOutput(program);
    auto output = RunProgram(program, rega, regb, regc);
    res1 = StringifyOutput(output);


    // StringifyOutput(program);
    res2 = FindA(program, program.size() - 1, 0, 0, 0).value();

    // auto outp = RunProgram(program, res2, 0, 0);
    // StringifyOutput(outp);

    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
