#include "advent.hpp"

enum struct TokenKind {
    MUL,
    DO,
    DONT,
    END,
};

struct Token {
    TokenKind kind;
    size_t start;
    size_t end;
    struct {
        int first;
        int second;
    } mul;
};


#define EXPECT(v) c = stream.peek(); \
    if (c != v) { goto read; } \
    c = stream.get()



Token GetToken(istringstream &stream) {
    Token res = {};

read:
    if (!stream.good()) {
        res.kind = TokenKind::END;
        return res;
    }
    char c = stream.get();

    if (c == 'm') {
        res.start = (int) stream.tellg() - 1;
        EXPECT('u');
        EXPECT('l');
        EXPECT('(');
        assert(std::isdigit(stream.peek()));
        int first = 0;
        stream >> first;
        EXPECT(',');
        assert(std::isdigit(stream.peek()));
        int second = 0;
        stream >> second;
        EXPECT(')');
        res.kind = TokenKind::MUL;
        res.mul.first = first;
        res.mul.second = second;
        res.end = stream.tellg();
        return res;
    } else if (c == 'd') {
        res.start = (int) stream.tellg() - 1;
        EXPECT('o');
        if (stream.peek() == '(') {
            // do
            stream.get();
            EXPECT(')');
            res.kind = TokenKind::DO;
            res.end = stream.tellg();
            return res;
        } else {
            // don't
            EXPECT('n');
            EXPECT('\'');
            EXPECT('t');
            EXPECT('(');
            EXPECT(')');
            res.kind = TokenKind::DONT;
            res.end = stream.tellg();
            return res;
        }
    }

    goto read;


    return res;
}

int main() {
    auto fp = ifstream("data/day03.txt");

    int res1 = 0;
    int res2 = 0;
    bool enabled = true;
    for (string line; getline(fp, line);) {
        istringstream stream(line);
        Token token = {};
        while ((token = GetToken(stream)).kind != TokenKind::END) {
            string value = line.substr(token.start, token.end - token.start);
            switch (token.kind) {
                case TokenKind::MUL: {
                    int value = token.mul.first * token.mul.second;
                    res1 += value;
                    res2 += enabled ? value : 0;
                } break;
                case TokenKind::DO:
                    enabled = true;
                    break;
                case TokenKind::DONT: {
                    enabled = false;
                } break;
            }
        }
    }
    println("Solution1: {}", res1);
    println("Solution2: {}", res2);
    return 0;
}
