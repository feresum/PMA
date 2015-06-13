#ifndef H_PARSE
#define H_PARSE

#include "general.h"
#include "pattern.h"

struct s_i {
    const str &s;
    size_t i;
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
public:
    const char *msg;
    int i;

    parse_exc(const char *msg, int i = -1) : msg(msg), i(i) { }

    const char *what() { return msg; }
};

P_Sequence* parse(s_i &prog);

bool is_dir_inst(int ch, bool allowrel);
vector<Direction> read_dirs(s_i &x, bool allowrel);


#endif
