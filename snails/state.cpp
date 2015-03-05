#include "state.h"



State::State(Grid g) : cg(g) { }
State::State(State &o) :
    cg(o.cg), direction(o.direction), position(o.position), flags(o.flags) { }

void State::advance() {
    position = position + direction;
    cg.setslime(position, 1);
}






StateP::StateP(State st, P_Sequence *seq, int iseq) :
        st(st), seq(seq), iseq(iseq) { }

StateP::StateP(StateP &o) : st(o.st), seq(o.seq), iseq(o.iseq) { }

