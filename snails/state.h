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

    State(Grid g);
    State(State &o);

   void advance();
};

class P_Sequence;

class StateP {
public:
    State st;
    P_Sequence *seq;
    int iseq;

    StateP(State st, P_Sequence *seq, int iseq);

    StateP(StateP &o);

};

#endif
