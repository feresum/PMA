#ifndef H_PARSE
#define H_PARSE

#include "general.h"
#include "pattern.h"

struct s_i {
    const str &s;
    unsigned i;
    int get() {
        i += 1;
        return i <= s.length() ? s[i-1] : -1;
    }
    int peek(int d = 0) {
        return i < s.length() ? s[i + d] : -1;
    }
    void back(int d) {
        i -= d;
    }
};

class parse_exc : std::exception {
    char *msg;
    int i;

    parse_exc(char *msg, int i) : msg(msg), i(i) { }

    char *what() { return msg; }
};

P_Sequence* parse(s_i &prog);


#endif
