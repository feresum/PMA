#include "interpreter.h"
#include "state.h"
#include "parse.h"
#include "instructions.h"

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

void interpreter(const str &program, const str &input, std::ostream &out, std::ostream &err) {
    
    int nl = program.find('\n');
    m_just just = M_JUST_LEFT; m_start start = M_START_ALLBOX; m_type type = M_TYPE_COUNT;
    if (~nl) {
        str opts = program.substr(0, nl);
        s_i o{ opts, 0 };
        for (int c; ~(c = o.get());) {
            if (c == INST_OPTION_BOOLEAN) type = M_TYPE_BOOLEAN;
            else if (c == INST_OPTION_RJUST) just = M_JUST_RIGHT;
        }
    }

    s_i body{ program, nl + 1 };
    P_Sequence *pat;
    try {
        pat = parse(body);
    } catch (parse_exc e) {
        err << "Parse error: " << e.what();
        if (~e.i) err << " at position " << e.i;
        err << std::endl;
        return;
    }

    State global(Grid{ input, '\n', just, OUT_CHAR });

    int nmatch = 0;

    for (int y = 0; y < global.cg.bHeight(); y++) {
        for (int x = 0; x < global.cg.bWidth(); x++) {
            State local{ global };
            local.direction = { 1, 0 };
            local.position = { x, y };
            match_result mr = match(local, pat);
            if (mr == MATCH_RESULT_SUCCESS) {
                if (type == M_TYPE_BOOLEAN) {
                    out << 1;
                    return;
                }
                nmatch++;
            } else if (mr == MATCH_RESULT_OVERFLOW) {
                err << "Stack Overflow\n";
                return;
            }
        }
    }
    out << nmatch;
}