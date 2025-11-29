#include <cstddef>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdint>
#include <ios>

using namespace std;

struct ComplexNumber {
    int64_t x;
    int64_t y;
};

ComplexNumber operator+(const ComplexNumber& a, const ComplexNumber &b) {
    return {a.x + b.x, a.y + b.y};
}

// [X1,Y1] * [X2,Y2] = [X1 * X2 - Y1 * Y2, X1 * Y2 + Y1 * X2]
ComplexNumber operator*(const ComplexNumber& a, const ComplexNumber &b) {
    return {a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x};
}

// [X1,Y1] / [X2,Y2] = [X1 / X2, Y1 / Y2]
ComplexNumber operator/(const ComplexNumber& a, const ComplexNumber &b) {
    return {a.x / b.x, a.y / b.y};
}

std::istream& operator>>(std::istream& in, ComplexNumber& number) {
    char c = in.get(); assert(c == '[');
    in >> number.x;
    c = in.get(); assert(c == ',');
    in >> number.y;
    c = in.get(); assert(c == ']');
    return in;
}

std::ostream& operator<<(std::ostream& out, const ComplexNumber& number) {
    out << "[" << number.x << "," << number.y << "]";
    return out;
}

ComplexNumber loop(const ComplexNumber number, const ComplexNumber a, const ComplexNumber&& divisor) {
    ComplexNumber res = number;
    res = res * res;
    res = res / divisor;
    res = res + a;
    return res;
}

ComplexNumber read_from_file(string&& filepath) {
    std::ifstream fp(filepath);
    assert(fp.is_open());
    fp.seekg(2, std::ios::cur);
    ComplexNumber res{};
    fp >> res;
    return res;
}

void quest1() {
    ComplexNumber a = read_from_file("./data/2025_q02_p1.txt");
    ComplexNumber res{};
    for (int i = 0; i < 3; ++i) {
        res = loop(res, a, {10, 10});
    }
    cout << res << "\n";
}

optional<ComplexNumber> verify(const ComplexNumber number) {
    ComplexNumber res{0, 0};
    for (int i = 0; i < 100; ++i) {
        res = loop(res, number, {100000, 100000});
        constexpr int64_t threshold = 1000000;
        if (llabs(res.x) > threshold || llabs(res.y) > threshold) {
            return nullopt;
        }
    }
    return {res};
}

void quest2() {
    ComplexNumber start = read_from_file("./data/2025_q02_p2.txt");
    ComplexNumber end_offset = {1000, 1000};
    ComplexNumber end = start + end_offset;

    ComplexNumber dim{100, 100};
    ComplexNumber offset = end_offset / dim;

    uint64_t res = 0;
    for (int y = 0; y <= dim.y; ++y) {
        for (int x = 0; x <= dim.x; ++x) {
            ComplexNumber p = {
                .x=start.x + offset.x * x,
                .y=start.y + offset.y * y,
            };

            auto value = verify(p);
            char c = '.';
            if (value) {
                c = 'x';
                res++;
            }
            // cout << c;
        }
        // cout << "\n";
    }
    cout << res << "\n";

}

int main() {
    quest1();
    quest2();

}
