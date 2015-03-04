#include "parse.h"
#include "pattern.h"
#include "instructions.h"
#include "tokens.h"

int read_ch(s_i &x) {
    return x.get();
}

bool digit(int ch) {
    return ch >= '0' && ch <= '9';
}

int read_num(s_i &x) {
    int num = 0;
    while (digit(x.peek())) {
        num = num * 10 + (x.get() - '0');
    }
    return num;
}

std::vector<PS_Direction> read_dirs(s_i &x) {

}

std::vector<point> dirs(int ch) {
    switch (ch) {
        case ''
    }
}

std::vector<Token> parse0(s_i &x) {
    std::vector<Token> v;
    int ch;
    while (~(ch = x.get())) {
        switch (ch) {
        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
            x.back(1);
            unsigned ll = read_num(x), ul;
            if (x.peek() == INST_QUANTIFIER_RANGE) {
                x.get();
                ul = digit(x.peek()) ? read_num(x) : ~0;
            } else {
                ul = ll;
            }
            v.push_back(T_Quantifier{ ll, ul });
            break;

        case INST_QUANTIFIER_RANGE:
            ul = digit(x.peek()) ? read_num(x) : ~0;
            v.push_back(T_Quantifier{ 0, ul });
            break;
        case INST_CHAR_LITERAL:
            P_CharExact hh{ 5 };

            v.push_back(T_Pattern{ P_CharExact{ x.get() } });
        }
    }
}

Pattern parse(s_i &prog) {

}