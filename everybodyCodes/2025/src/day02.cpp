#include <iostream>
#include <fstream>
#include <cassert>
#include <ios>

using namespace std;

struct ComplexNumber {
    int x;
    int y;
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

ComplexNumber loop(const ComplexNumber number, const ComplexNumber a) {
    ComplexNumber res = number;
    res = res * res;
    res = res / ComplexNumber{10, 10};
    res = res + a;
    return res;
}


int main() {

    std::ifstream fp("./data/everybody_codes_e2025_q02_p1.txt");
    assert(fp.is_open());
    fp.seekg(2, std::ios::cur);

    ComplexNumber a{};
    ComplexNumber res{};
    fp >> a;
    for (int i = 0; i < 3; ++i) {
        res = loop(res, a);
    }
    cout << res << "\n";
}
