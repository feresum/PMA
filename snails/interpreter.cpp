#include "interpreter.h"
#include "state.h"
#include "parse.h"

void interpreter(str &program, str &input, std::ostream &out, std::ostream &err) {
    
    int nl = program.find('\n');
    m_just just = M_JUST_LEFT; m_start start = M_START_ALLBOX; m_type type = M_TYPE_COUNT;
    if (~nl) {
        str opts = program.substr(0, nl);
        s_i o{ opts, 0 };
        for (int c; ~(c = o.get());) {

        }
    }

    //State globalSt();

    int nmatch = 0;
    for (int x = 0; x < 3;);
}