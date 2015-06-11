#include "state.h"
#include "execute.h"

int match(State initial, P_Sequence *p, bool allpaths) {
    int nmatch = 0;
    vector<StateP> stk;
    stk.push_back(StateP{ initial, p, 0 });
    while (!stk.empty()) {
        auto &bk = stk.back();
        if (bk.iseq == bk.seq->v.size()) {
            bk.iseq = bk.seq->iparent + 1;
            bk.seq = bk.seq->parent;
        } else {
            if (bk.seq->v.at(bk.iseq)->match(stk) == 1) {
                if (!allpaths) return 1;
                nmatch++;
                stk.pop_back();
            }
            if (stk.size() > MAX_PATTERN_STACK) {
                return MATCH_RESULT_OVERFLOW;
            }
        }
    }
    return nmatch;
}