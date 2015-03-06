#ifndef H_STATE
#define H_STATE

#include "grid.h"
#include "pattern.h"

class State {
public:
    Grid cg;
    point direction;
    point position;
    int flags;

    State(const Grid &g);
    inline State(const State &o) = default;

    point nextPos();
    int nextChar();
    bool canAdvance();
    void advance();
};

class P_Sequence;

class StateP {
public:
    State st;
    P_Sequence *seq;
    int iseq;

    StateP(State st, P_Sequence *seq, int iseq);

    inline StateP(const StateP &o) = default;

};

#define SF_NON_MOVE_MATCH 0x1

#endif
