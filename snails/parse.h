#ifndef H_PARSE
#define H_PARSE

#include "general.h"
#include "pattern.h"
#include "encoding.h"


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
void skip_comment(s_i &x);

#endif
