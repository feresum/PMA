#include "pattern.h"
#include "interpreter.h"
#include "execute.h"

point PS_DirAbsolute::getDir(point prev) {
    return dir;
}

point PS_DirRelative::getDir(point prev) {
    const int m1[] = { 1, 1, 0, -1, -1, -1, 0, 1};
    int c = m1[angle], s = m1[angle + 6 & 7];
    point r = { c*prev.x + s*prev.y, c*prev.y - s*prev.x };
    if (!((r.x | r.y) & 1)) {
        r.x /= 2;
        r.y /= 2;
    }
    return r;
}

int P_DirectionAlternation::match(vector<StateP> &stk) {
    StateP sp = stk.back(); stk.pop_back();

    vector<point> dirs;
    for (size_t i = 0; i < list.size(); i++) {
        point d = list[i]->getDir(sp.st.direction);
        if (std::find(dirs.begin(), dirs.end(), d) == dirs.end()) {
            dirs.push_back(d);
        }
    }
    sp.iseq++;
    for (size_t i = dirs.size(); i--;) {
        sp.st.direction = dirs[i];
        stk.push_back(sp);
    }
    return 0;
}

int P_Alternation::match(vector<StateP> &stk) {
    StateP sp = stk.back(); stk.pop_back();

    for (size_t i = v.size(); i--; ) {
        stk.push_back(sp);
        v[i]->match(stk);
    }
    return 0;
}

int P_Char::match(vector<StateP> &stk) {
    State &st = stk.back().st;
    if (testch(st.nextChar()) && st.canAdvance()) {
        st.advance();
        stk.back().iseq++;
    } else {
        stk.pop_back();
    }
    return 0;
}

int P_Terminator::match(vector<StateP> &stk) {
    return 1;
}

int P_Sequence::match(vector<StateP> &stk) {
    auto &bk = stk.back();
    //TODO lolol
    parent = bk.seq; 
    iparent = bk.iseq;
    bk.seq = this;
    bk.iseq = 0;
    return 0;
}

int P_Jump::match(vector<StateP> &stk) {
    stk.back().iseq += offset;
    return 0;
}

int P_Quantifier::match(vector<StateP> &stk) {
    StateP sp = stk.back(); stk.pop_back();
    unsigned count = sp.st.qcount.back();
    if (count >= minimum) {
        stk.push_back(sp);
        stk.back().iseq += offset;
        stk.back().st.qcount.pop_back();
    } 
    if (count < maximum) {
        stk.push_back(sp);
        stk.back().iseq++;
        stk.back().st.qcount.back()++;
    }
    return 0;
}

int P_QReset::match(vector<StateP> &stk) {
    stk.back().st.qcount.push_back(0);
    stk.back().iseq++;
    return 0;
}

int P_Assertion::match(vector<StateP> &stk) {
    if ((int) ::match(stk.back().st, pattern) == (int) value) {
        stk.back().iseq++;
    } else {
        stk.pop_back();
    }
    return 0;
}