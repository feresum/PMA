#include <typeinfo>

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

vector<PS_Direction> read_dirs(s_i &x) {
    vector<PS_Direction> v;
    while (1) {
        char ch = x.get();
        switch (ch) {
        case INST_D_RIGHT:
            v.push_back(PS_DirAbsolute{ point{ 1, 0 } });
            break;
        case INST_D_UP:
            v.push_back(PS_DirAbsolute{ point{ 0, 1 } });
            break;
        case INST_D_LEFT:
            v.push_back(PS_DirAbsolute{ point{ -1, 0 } });
            break;
        case INST_D_DOWN:
            v.push_back(PS_DirAbsolute{ point{ 0, -1 } });
            break;
        case INST_D_ORTHOGONAL:
            v.push_back(PS_DirAbsolute{ point{ 1, 0 } });
            v.push_back(PS_DirAbsolute{ point{ 0, 1 } });
            v.push_back(PS_DirAbsolute{ point{ -1, 0 } });
            v.push_back(PS_DirAbsolute{ point{ 0, -1 } });
            break;
        case INST_D_DIAGONAL:
            v.push_back(PS_DirAbsolute{ point{ 1, 1 } });
            v.push_back(PS_DirAbsolute{ point{ -1, 1 } });
            v.push_back(PS_DirAbsolute{ point{ -1, -1 } });
            v.push_back(PS_DirAbsolute{ point{ 1, -1 } });
            break;
        case INST_D_OCTILINEAR:
            v.push_back(PS_DirAbsolute{ point{ 0, 0 } });
            v.push_back(PS_DirAbsolute{ point{ 1, 1 } });
            v.push_back(PS_DirAbsolute{ point{ 0, 1 } });
            v.push_back(PS_DirAbsolute{ point{ -1, 1 } });
            v.push_back(PS_DirAbsolute{ point{ -1, 0 } });
            v.push_back(PS_DirAbsolute{ point{ -1, -1 } });
            v.push_back(PS_DirAbsolute{ point{ 0, -1 } });
            v.push_back(PS_DirAbsolute{ point{ 1, -1 } });
            break;
        case INST_D_CLOCKWISE:
            v.push_back(PS_DirRelative{ 6 });
            break;
        case INST_D_COUNTERCLOCKWISE:
            v.push_back(PS_DirRelative{ 2 });
            break;
        case INST_D_FORWARD:
            v.push_back(PS_DirRelative{ 0 });
            break;
        case INST_D_BACKWARD:
            v.push_back(PS_DirRelative{ 4 });
            break;
        case INST_D_NORMAL:
            v.push_back(PS_DirRelative{ 2 });
            v.push_back(PS_DirRelative{ 6 });
            break;
        case INST_D_STATIONARY:
            v.push_back(PS_DirAbsolute{ point{ 0, 0 } });
            break;
        case INST_D_ABSNUM:
            //v.push_back(PS_Dir; TODO
            break;
        case INST_D_RELNUM:
            v.push_back(PS_DirRelative{ read_num(x) });
            break;
        default:
            x.back(1);
            return v;
        }
    }
}




vector<Token> parse0(s_i &x) {
    vector<Token> v;
    int ch;
    while (~(ch = x.get())) {
        switch (ch) {
        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        {
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
        }

        case INST_QUANTIFIER_RANGE:
        {
            unsigned ul = digit(x.peek()) ? read_num(x) : ~0;
            v.push_back(T_Quantifier{ 0, ul });
            break;
        }

        case INST_CHAR_LITERAL:
            v.push_back(T_Pattern{ P_CharExact{ x.get() } });
            break;

        case INST_ALTERNATION:
            v.push_back(T_Alternator{});
            break;

        case INST_GROUP_OPEN1:
            v.push_back(T_GroupOpen{ 1 });
            break;

        case INST_GROUP_OPEN2:
            v.push_back(T_GroupOpen{ 2 });
            break;

        case INST_GROUP_CLOSE1:
            v.push_back(T_GroupClose{ 1 });
            break;

        case INST_GROUP_CLOSE2:
            v.push_back(T_GroupClose{ 2 });
            break;

        case INST_GROUP_DIV:
            v.push_back(T_GroupDiv{});
            break;

        case INST_D_RIGHT:
        case INST_D_UP:
        case INST_D_LEFT:
        case INST_D_DOWN:
        case INST_D_ORTHOGONAL:
        case INST_D_DIAGONAL:
        case INST_D_OCTILINEAR:
        case INST_D_CLOCKWISE:
        case INST_D_COUNTERCLOCKWISE:
        case INST_D_FORWARD:
        case INST_D_BACKWARD:
        case INST_D_NORMAL:
        case INST_D_STATIONARY:
        case INST_D_ABSNUM:
        case INST_D_RELNUM:
            v.push_back(T_DirList{ read_dirs(x) });
            break;

        }//switch
    
    }
    return v;
}

P_Sequence parse_group(vector<Token> &t, int start, int end) {
    vector<Token> t2;
    struct gi { int g, i; };
    vector<gi> groups;
    for (int i = start; i < end; i++) {
        T_GroupOpen *open;
        T_GroupClose *close;
        //TODO: t_groupdiv
        if (open = dynamic_cast<T_GroupOpen*>(&t.at(i))) {
            groups.push_back({ open->type, i });
        } else if (close = dynamic_cast<T_GroupClose*>(&t.at(i)))  {
            while (groups.size() && groups.back().g != close->type) {
                groups.pop_back();
            }
            int i0 = start;
            if (groups.size()) {
                i0 = groups.back().i + 1;
                groups.pop_back();
            }
            t2.push_back(T_Pattern{ parse_group(t, i0, i) });
        } else {
            t2.push_back(t[i]);
        }
    } 
    if (groups.size()) {
        t2.push_back(T_Pattern{ parse_group(t, groups[0].i+1, end) });
    }
    //vector<int> alts;

    P_Sequence r;
    for (auto tok : t2) {
        if (typeid(tok) == typeid(T_Pattern)) {
            r.v.push_back(dynamic_cast<Pattern&>(tok));
        }
    }
    return r;
}

P_Sequence parse(s_i &prog) {
    auto toks = parse0(prog);
    P_Sequence res = parse_group(toks, 0, toks.size());
    res.v.push_back(P_Terminator{});
    return res;
}