#include "interpreter.h"
#include "state.h"
#include "parse.h"
#include "instructions.h"
#include "execute.h"
#include "encoding.h"

void interpreter(const str &program, const str &input, std::ostream &out, std::ostream &err) {
    encoding_t progEnc = detect_encoding(program);
    size_t nl = program.find('\n');
    m_just just = M_JUST_LEFT; m_start start = M_START_ALLBOX; m_type type = M_TYPE_COUNT;
    int chfill = OUT_CHAR;
    bool mobile = false, allpaths = false;
    vector<Direction> dirs;
    dirs.emplace_back( point{ 1, 0 } );

    if (~nl) {
        s_i o{ program, progEnc == ENC_UTF8 };
        for (int c; o.ind() < nl; ) {
            c = o.get();
            if (c == INST_OPTION_TYPE_BOOLEAN) type = M_TYPE_BOOLEAN;
            else if (c == INST_OPTION_TYPE_ALL) type = M_TYPE_ALL;
            else if (c == INST_OPTION_RJUST) just = M_JUST_RIGHT;
            else if (c == INST_OPTION_START_TOPLEFT) start = M_START_TOPLEFT;
            else if (c == INST_OPTION_FILL_SPACE) chfill = ' ';
            else if (c == INST_OPTION_FILL_CHAR) chfill = o.get();
            else if (c == INST_OPTION_MOBILE_START) mobile = true;
            else if (c == INST_OPTION_ALL_PATHS) allpaths = true;
			else if (c == INST_QUANTIFIER_RANGE_OR_LINE_COMMENT && o.peek() == INST_QUANTIFIER_RANGE_OR_LINE_COMMENT) skip_comment(o);
            else if (is_dir_inst(c, false)) {
                o.back();
                dirs = read_dirs(o, false);
            } else if (!isspace(c)) {
                err << "Unrecognized option: " << c;
                return;
            }
        }
    }

    s_i body{ program, progEnc == ENC_UTF8, nl + 1 };
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
    State global(Grid{ s_i{ input, progEnc != ENC_SINGLE_BYTE && detect_encoding(input) != ENC_SINGLE_BYTE },
        '\n', just, chfill, empty }, mobile);
    if (empty && type == M_TYPE_ALL) {
        out << 1;
        return;
    }

    int nmatch = 0;

    int xmax = start == M_START_ALLBOX ? global.cg.bWidth() : 1;
    int ymax = start == M_START_ALLBOX ? global.cg.bHeight() : 1;
    for (int y = 0; y < ymax; y++) {
        for (int x = 0; x < xmax; x++) {
            for (Direction &d : dirs) {
                State local{ global };
                local.direction = d.absdir();
                local.position = { x, y };
                int mr = match(local, pat, allpaths);
                if (mr > 0) {
                    if (type == M_TYPE_BOOLEAN) {
                        out << 1;
                        return;
                    }
                    nmatch += mr;
                } else if (mr == MATCH_RESULT_OVERFLOW) {
                    err << "Stack Overflow\n";
                    return;
                } else if (type == M_TYPE_ALL) {
                    out << 0;
                    return;
                }
            }
        }
    }
    if (type == M_TYPE_ALL) {
        out << 1;
    } else {
        out << nmatch;
    }
}
