#include <typeinfo>
#include <cctype>

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

vector<PS_Direction*> read_dirs(s_i &x) {
    vector<PS_Direction*> v;
    while (1) {
        char ch = x.get();
        switch (ch) {
        case INST_D_RIGHT:
            v.push_back(new PS_DirAbsolute{ point{ 1, 0 } });
            break;
        case INST_D_UP:
            v.push_back(new PS_DirAbsolute{ point{ 0, -1 } });
            break;
        case INST_D_LEFT:
            v.push_back(new PS_DirAbsolute{ point{ -1, 0 } });
            break;
        case INST_D_DOWN:
            v.push_back(new PS_DirAbsolute{ point{ 0, 1 } });
            break;
        case INST_D_ORTHOGONAL:
            v.push_back(new PS_DirAbsolute{ point{ 1, 0 } });
            v.push_back(new PS_DirAbsolute{ point{ 0, -1 } });
            v.push_back(new PS_DirAbsolute{ point{ -1, 0 } });
            v.push_back(new PS_DirAbsolute{ point{ 0, 1 } });
            break;
        case INST_D_DIAGONAL:
            v.push_back(new PS_DirAbsolute{ point{ 1, -1 } });
            v.push_back(new PS_DirAbsolute{ point{ -1, -1 } });
            v.push_back(new PS_DirAbsolute{ point{ -1, 1 } });
            v.push_back(new PS_DirAbsolute{ point{ 1, 1 } });
            break;
        case INST_D_OCTILINEAR:
            for (point p : DIRECTION_LIST) {
                v.push_back(new PS_DirAbsolute{ p });
            }
            break;
        case INST_D_CLOCKWISE:
            v.push_back(new PS_DirRelative{ 6 });
            break;
        case INST_D_COUNTERCLOCKWISE:
            v.push_back(new PS_DirRelative{ 2 });
            break;
        case INST_D_FORWARD:
            v.push_back(new PS_DirRelative{ 0 });
            break;
        case INST_D_BACKWARD:
            v.push_back(new PS_DirRelative{ 4 });
            break;
        case INST_D_NORMAL:
            v.push_back(new PS_DirRelative{ 2 });
            v.push_back(new PS_DirRelative{ 6 });
            break;
        case INST_D_STATIONARY:
            v.push_back(new PS_DirAbsolute{ point{ 0, 0 } });
            break;
        case INST_D_ABSNUM:
            if (!digit(x.peek())) throw parse_exc("Missing digit for absolute direction", x.i);
            v.push_back(new PS_DirAbsolute(DIRECTION_LIST[x.get() & 7]));
            break;
        case INST_D_RELNUM:
            v.push_back(new PS_DirRelative{ digit(x.peek())? x.get() & 7: 1 });
            break;
        default:
            x.back(1);
            return v;
        }
    }
}




vector<Token*> parse0(s_i &x) {
    vector<Token*> v;
    int ch;
    while (~(ch = x.get())) {
        switch (ch) {
        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        {
            x.back(1);
            unsigned ll = read_num(x), ul;
            if (x.peek() == INST_QUANTIFIER_RANGE) {
                x.get();
                ul = digit(x.peek()) ? read_num(x) : ~0U;
            } else {
                ul = ll;
            }
            v.push_back(new T_Quantifier{ ll, ul });
            break;
        }

        case INST_QUANTIFIER_RANGE:
        {
            unsigned ul = digit(x.peek()) ? read_num(x) : ~0U;
            v.push_back(new T_Quantifier{ 0, ul });
            break;
        }

        case INST_QUANTIFIER_AT_LEAST_1:
            v.push_back(new T_Quantifier{ 1, ~0U });
            break;

        case INST_QUANTIFIER_AT_MOST_1:
            v.push_back(new T_Quantifier{ 0, 1 });
            break;

        case INST_CHAR_LITERAL:
            v.push_back(new T_Pattern{ new P_CharExact{ x.get() } });
            break;

        case INST_CHAR_NEGATIVE_LITERAL:
            v.push_back(new T_Pattern{ new P_CharNegative{ x.get() } });
            break;

        case INST_CHAR_ANY:
            v.push_back(new T_Pattern{ new P_CharAny });
            break;

        case INST_CHAR_OUT:
            v.push_back(new T_Pattern{ new P_CharOut });
            break;

        case INST_ALTERNATION:
            v.push_back(new T_Alternator{});
            break;

        case INST_GROUP_OPEN1:
            v.push_back(new T_GroupOpen{ 1 });
            break;

        case INST_GROUP_OPEN2:
            v.push_back(new T_GroupOpen{ 2 });
            break;

        case INST_GROUP_CLOSE1:
            v.push_back(new T_GroupClose{ 1 });
            break;

        case INST_GROUP_CLOSE2:
            v.push_back(new T_GroupClose{ 2 });
            break;

        case INST_GROUP_DIV:
            v.push_back(new T_GroupDiv{});
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
            x.back(1);
            v.push_back(new T_Pattern{ new P_DirectionAlternation{ read_dirs(x) } });
            break;

        case INST_ASSERT_POSITIVE:
            v.push_back(new T_Assert{ true });
            break;
        case INST_ASSERT_NEGATIVE:
            v.push_back(new T_Assert{ false });
            break;

        default:
            if (!isspace(ch)) {
                throw parse_exc("Unrecognized instruction", x.i);
            }
        }//switch
    
    }
    return v;
}




P_Sequence * parse_group(vector<Token*> &t, size_t start, size_t end) {
    vector<Token*> t2;
    struct gi { int g; size_t i; };
    vector<gi> groups;
    for (size_t i = start; i < end; i++) {
        T_GroupOpen *open;
        T_GroupClose *close;
        
        if (open = dynamic_cast<T_GroupOpen*>(t.at(i))) {
            groups.push_back({ open->type, t2.size() });
        } else if (close = dynamic_cast<T_GroupClose*>(t.at(i)))  {
            while (groups.size() && groups.back().g != close->type) {
                size_t i0 = groups.back().i;
                groups.pop_back();
                T_Pattern *gr = new T_Pattern{ parse_group(t2, i0, t2.size()) };
                t2.erase(t2.begin() + i0, t2.end());
                t2.push_back(gr);
            }
            size_t i0 = 0;
            if (groups.size()) {
                i0 = groups.back().i;
                groups.pop_back();
            }
            T_Pattern *gr = new T_Pattern{ parse_group(t2, i0, t2.size()) };
            t2.erase(t2.begin() + i0, t2.end());
            t2.push_back(gr);
        } else {
            t2.push_back(t[i]);
        }
    } 
    while (!groups.empty()) {
        size_t i0 = groups.back().i;
        groups.pop_back();
        T_Pattern *gr = new T_Pattern{ parse_group(t2, i0, t2.size()) };
        t2.erase(t2.begin() + i0, t2.end());
        t2.push_back(gr);
    }

    
    vector<size_t> alts;
    for (size_t i = 0; i < t2.size(); i++) {
        if (isA<T_Alternator>(t2[i])) {
            alts.push_back(i);
        } else if (isA<T_GroupDiv>(t2[i])) {
            size_t last = alts.empty() ? 0 : alts.back();
            t2[last + 1] = new T_Pattern{ parse_group(t2, last + 1, i) };
            t2.erase(t2 + last + 2, t2 + i + 1);
            i = last + 1;
        }
    }
    if (!alts.empty()) {
        P_Alternation *a = new P_Alternation;
        alts.push_back(t2.size());
        size_t last = 0;
        for (size_t i : alts) {
            a->v.push_back(parse_group(t2, last, i));
            last = i + 1;
        }
        P_Sequence *r = new P_Sequence;
        r->v.push_back(a);
        return r;
    }

    
    for (size_t i = 0; i < t2.size(); i++) {
        T_Quantifier *tq;
        if (tq = dynamic_cast<T_Quantifier*>(t2[i])) {
            if (!tq->target) {
                if (!i) {
                    throw parse_exc("Nothing to quantify");
                }
                tq->target = t2[i - 1];
                t2.erase(t2 + --i);
            }
        }
    }

    for (size_t i = 0; i < t2.size(); i++) {
        T_Assert *ass;
        if (ass = dynamic_cast<T_Assert*>(t2[i])) {
            if (i + 1 == t2.size()) {
                throw parse_exc("Nothing to assert");
            }
            P_Sequence *s = parse_group(t2, i + 1, i + 2);
            s->v.push_back(new P_Terminator);
            t2[i] = new T_Pattern(new P_Assertion(ass->value, s));

            t2.erase(t2 + i + 1);
        }
    }

    P_Sequence *r = new P_Sequence;

    for (size_t i = 0; i < t2.size(); i++) {
        int qlevel = 0;
        T_Quantifier *tq;
        while (tq = dynamic_cast<T_Quantifier*>(t2[i])) {
            r->v.push_back(new P_QReset);
            r->v.push_back(new P_Quantifier(tq->minimum, tq->maximum));
            qlevel++;
            t2[i] = tq->target;
        }
        if (!isA<T_Pattern>(t2[i])) {
            throw parse_exc("Nothing to quantify");
        }
        r->v.push_back(((T_Pattern*)(t2[i]))->p);
        for (int j = 1; j <= qlevel; j++) {
            ((P_Quantifier*)(r->v[r->v.size() + 1 - 3*j]))->offset = 3*j;
            r->v.push_back(new P_Jump{ 1 - 3*j });
        }
    }

    return r;
}

P_Sequence * parse(s_i &prog) {
    auto toks = parse0(prog);
    Pattern *res = parse_group(toks, 0, toks.size());
    P_Sequence *ps;
    if (isA<P_Sequence>(res)) ps = (P_Sequence*)res;
    else {
        ps = new P_Sequence;
        ps->v.push_back(res);
    }
    ps->v.push_back(new P_Terminator);
    return ps;
}

