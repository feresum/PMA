#include "state.h"
#include "execute.h"

match_result match(State initial, P_Sequence *p) {
    vector<StateP> stk;
    stk.push_back(StateP{ initial, p, 0 });
    while (!stk.empty()) {
        auto &bk = stk.back();
        if (bk.iseq == bk.seq->v.size()) {
            bk.iseq = bk.seq->iparent + 1;
            bk.seq = bk.seq->parent;
        } else {
            if (bk.seq->v.at(bk.iseq)->match(stk) == 1) {
                return MATCH_RESULT_SUCCESS;
            }
            if (stk.size() > MAX_PATTERN_STACK) {
                return MATCH_RESULT_OVERFLOW;
            }
        }
    }
    return MATCH_RESULT_FAIL;
}