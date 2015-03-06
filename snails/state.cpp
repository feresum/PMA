#include "state.h"



State::State(const Grid &g) : cg(g), flags(SF_NON_MOVE_MATCH) { }

point State::nextPos() {
    if (flags & SF_NON_MOVE_MATCH) {
        return position;
    } else {
        return position + direction;
    }
}
int State::nextChar() {
    return cg(nextPos());
}

bool State::canAdvance() {
    return !cg.slime(nextPos());
}

void State::advance() {
    position = nextPos();
    cg.setslime(position, 1);
    flags &= ~SF_NON_MOVE_MATCH;
}






StateP::StateP(State st, P_Sequence *seq, int iseq) :
        st(st), seq(seq), iseq(iseq) { }



