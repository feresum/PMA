#ifndef H_PATTERN
#define H_PATTERN

#include "state.h"
#include "general.h"

class StateP;


class Pattern {
public:
    virtual int match(vector<StateP> &stk);
};


class P_Char : public Pattern { 
    virtual bool testch(int ch);
public:
    int match(vector<StateP> &stk);
};

class P_CharAny : public P_Char { };

class P_CharExact : public P_Char { 
public: 
    int c;
    P_CharExact(int c) : c(c) { }
};

class P_Sequence : public Pattern {
public:
    vector<Pattern> v;
    P_Sequence *parent;

    int match(vector<StateP> &stk);
};

class P_Alternation : public Pattern {};

class P_Assertion : public Pattern { public: bool value; };


class P_SetState : public Pattern {
    
};

class PS_Direction : public P_SetState {
public:
    virtual point getDir(point prevdir);
};

class P_DirectionAlternation : public Pattern {
public:
    vector<PS_Direction> list;
    P_DirectionAlternation(vector<PS_Direction> list) : list(list) { }

    int match(vector<StateP> &stk);
};


class PS_DirAbsolute : public PS_Direction { 
public: 
    point dir; 
    point getDir(point prevdir);
    PS_DirAbsolute(point dir) : dir(dir) { }
};


class PS_DirRelative : public PS_Direction { 
    int angle;
public: 
    point getDir(point prevdir);
    PS_DirRelative(int angle) : angle(angle & 7) { }
};

class P_Terminator : public Pattern {
public:
    int match(vector<StateP> &stk);
};

#endif
