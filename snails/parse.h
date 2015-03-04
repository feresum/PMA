#include "general.h"

struct s_i {
    str &s;
    int i;
    int get() {
        return i < s.length() ? s[i++] : -1;
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