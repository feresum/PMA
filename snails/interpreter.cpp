#include "interpreter.h"
#include "state.h"
#include "parse.h"
#include "instructions.h"
#include "execute.h"

void interpreter(const str &program, const str &input, std::ostream &out, std::ostream &err) {
    
    int nl = program.find('\n');
    m_just just = M_JUST_LEFT; m_start start = M_START_ALLBOX; m_type type = M_TYPE_COUNT;
    int chfill = OUT_CHAR;
    if (~nl) {
        s_i o{ program, 0 };
        for (int c; o.i < nl; ) {
            c = o.get();
            if (c == INST_OPTION_TYPE_BOOLEAN) type = M_TYPE_BOOLEAN;
            else if (c == INST_OPTION_TYPE_ALL) type = M_TYPE_ALL;
            else if (c == INST_OPTION_RJUST) just = M_JUST_RIGHT;
            else if (c == INST_OPTION_START_TOPLEFT) start = M_START_TOPLEFT;
            else if (c == INST_OPTION_FILL_SPACE) chfill = ' ';
            else if (c == INST_OPTION_FILL_CHAR) chfill = o.get();
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
    bool empty;
    State global(Grid{ input, '\n', just, chfill, empty });
    if (empty && type == M_TYPE_ALL) {
        out << 1;
        return;
    }

    int nmatch = 0;

    int xmax = start == M_START_ALLBOX ? global.cg.bWidth() : 1;
    int ymax = start == M_START_ALLBOX ? global.cg.bHeight() : 1;
    for (int y = 0; y < ymax; y++) {
        for (int x = 0; x < xmax; x++) {
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
    if (type == M_TYPE_ALL) {
        out << +(nmatch == xmax * ymax);
    } else {
        out << nmatch;
    }
}