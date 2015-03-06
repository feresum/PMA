#ifndef H_PATTERN
#define H_PATTERN

#include "state.h"
#include "general.h"

class StateP;


class Pattern {
public:
    virtual int match(vector<StateP> &stk) { return NEVERHAPPEN };
};


class P_Char : public Pattern { 
public:
    virtual bool testch(int ch) { return NEVERHAPPEN };

    int match(vector<StateP> &stk);
};

class P_CharAny : public P_Char {
public:
    bool testch(int ch) { return ch >= 0; }
};

class P_CharExact : public P_Char { 
public: 
    int c;
    P_CharExact(int c) : c(c) { }

    bool testch(int ch) { return c == ch; }
};

class P_Sequence : public Pattern {
public:
    vector<Pattern*> v;
    P_Sequence *parent;
    int iparent;

    int match(vector<StateP> &stk);
};

class P_Alternation : public Pattern {};

class P_Assertion : public Pattern { public: bool value; };


class P_SetState : public Pattern {
    
};

class PS_Direction : public P_SetState {
public:
    virtual point getDir(point prevdir) { NEVERHAPPEN return{}; }
};

class P_DirectionAlternation : public Pattern {
public:
    vector<PS_Direction*> list;
    P_DirectionAlternation(vector<PS_Direction*> list) : list(list) { }

    int match(vector<StateP> &stk);
};


class PS_DirAbsolute : public PS_Direction { 
public: 
    point dir; 
    point getDir(point prevdir);
    PS_DirAbsolute(point dir) : dir(dir) { }

    int match(vector<StateP> &stk){ return 0; }
};


class PS_DirRelative : public PS_Direction { 
    int angle;
public: 
    point getDir(point prevdir);
    PS_DirRelative(int angle) : angle(angle & 7) { }

    int match(vector<StateP> &stk){ return 0; }
};

class P_Terminator : public Pattern {
public:
    P_Terminator() {
        int abc = 3;
        vector<StateP> bs;
        int x = match(bs);
        int y = 3;
    }
    int match(vector<StateP> &stk);
};

#endif
