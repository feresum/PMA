#ifndef H_PATTERN
#define H_PATTERN

#include "state.h"
#include "general.h"

class StateP;


class Pattern {
public:
    virtual int match(vector<StateP> &stk) { NEVERHAPPEN };
};


class P_Char : public Pattern { 
public:
    virtual bool testch(int ch) { NEVERHAPPEN };

    int match(vector<StateP> &stk);
};

class P_CharAny : public P_Char {
public:
    bool testch(int ch) { return ch >= 0; }
};

class P_CharOut : public P_Char {
public:
    bool testch(int ch) { return ch < 0; }
};

class P_CharExact : public P_Char { 
public: 
    int c;
    P_CharExact(int c) : c(c) { }

    bool testch(int ch) { 
        return c == ch; 
    }
};

class P_CharNegative : public P_Char {
public:
    int c;
    P_CharNegative(int c) : c(c) { }

    bool testch(int ch) {
        return ch >= 0 && c != ch;
    }
};

class P_Sequence : public Pattern {
public:
    vector<Pattern*> v;
    P_Sequence *parent;
    int iparent;

    int match(vector<StateP> &stk);
};

class P_Alternation : public Pattern {
public:
    vector<P_Sequence*> v;
    int match(vector<StateP> &stk);
};

class P_Assertion : public Pattern { 
public: 
    bool value; 
    P_Sequence *pattern;
    
    P_Assertion(bool value, P_Sequence *pattern) : value(value), pattern(pattern) { }

    int match(vector<StateP> &stk);
};


class P_SetState : public Pattern {
    
};

class Direction {
public:
    enum DType { ABSOLUTE, RELATIVE } type;
    int x[2];

    Direction(point dx): type(ABSOLUTE) {
        x[0] = dx.x;
        x[1] = dx.y;
    }
    Direction(int rdir): type(RELATIVE) {
        x[0] = rdir & 7;
    }
    Direction(const Direction &o) = default;

    point getDir(point prevdir); 

    point absdir() { return { x[0], x[1] }; }
};

class P_DirectionAlternation : public Pattern {
public:
    vector<Direction> list;
    P_DirectionAlternation(vector<Direction> list) : list(list) { }

    int match(vector<StateP> &stk);
};




class P_Teleport : public P_SetState {
    int match(vector<StateP> &stk);
};

class P_Terminator : public Pattern {
public:

    int match(vector<StateP> &stk);
};

class P_Jump : public Pattern {
public:
    int offset;
    P_Jump(int offset) : offset(offset) { }
    int match(vector<StateP> &stk);
};

class P_Quantifier : public Pattern {
public:
    unsigned minimum, maximum;
    bool reluctant;
    int offset;
    P_Quantifier(unsigned a, unsigned b) {
        minimum = std::min(a, b);
        maximum = std::max(a, b);
        reluctant = b < a;
    }
    int match(vector<StateP> &stk);
};

class P_QReset : public Pattern {
public:
    int match(vector<StateP> &stk);
};


#endif
